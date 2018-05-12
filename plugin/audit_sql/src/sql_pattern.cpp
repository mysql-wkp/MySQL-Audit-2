//use for loading mysql.sql_pattern catalog into cache.
#include <mysql_version.h>
#include "log.h"
#include "sql_pattern.h"

#if MYSQL_VERSION_ID >=50700
PSI_memory_key key_memory_sql_pattern;
#endif 

//for the name2pattern_cache
static HASH name2pattern_cache; 
static HASH pattern2name_cache;

MEM_ROOT mem_audit;
static mysql_rwlock_t THR_LOCK_sql_pattern;

#ifdef HAVE_PSI_INTERFACE
static PSI_rwlock_key key_rwlock_THR_LOCK_sql_pattern;

static PSI_rwlock_info all_sql_pattern_cache_rwlocks[]=
{
  { &key_rwlock_THR_LOCK_sql_pattern, "THR_LOCK_sql_pattern", PSI_FLAG_GLOBAL}
};

static void init_sql_pattern_cache_psi_keys(void)
{
  const char* category= "sql_pattern";
  int count;

  count= array_elements(all_sql_pattern_cache_rwlocks);
  mysql_rwlock_register(category, all_sql_pattern_cache_rwlocks, count);
}
#endif /* HAVE_PSI_INTERFACE */

bool isMapInitialized () 
{
	return my_hash_inited(&pattern2name_cache) && my_hash_inited(&name2pattern_cache);
}

#if MYSQL_VERSION_ID <= 50700
static uchar *name2pattern_cache_get_key(Name2Pattern* name2pattern, size_t *length,
                                    my_bool not_used __attribute__((unused)))
#else 
static uchar *name2pattern_cache_get_key(Name2Pattern* name2pattern, size_t *length,
                                    my_bool not_used MY_ATTRIBUTE((unused)))
#endif 
{
  *length = (uint) name2pattern->pattern_name_len;
  return (uchar*) name2pattern->pattern_name;
}

#if MYSQL_VERSION_ID <= 50700
static uchar *pattern2name_cache_get_key(Pattern2Name *pattern, size_t *length,
                                    my_bool not_used __attribute__((unused)))
#else
static uchar *pattern2name_cache_get_key(Pattern2Name *pattern, size_t *length,
                                    my_bool not_used MY_ATTRIBUTE((unused)))
#endif 
{
  *length = (uint) pattern-> sql_pattern_str_len;
  return (uchar*) pattern->sql_pattern_str;
}

bool sql_pattern_param_init (bool dont_read_table)
{
    bool return_val= FALSE;

#ifdef HAVE_PSI_INTERFACE
  init_sql_pattern_cache_psi_keys();
#endif

#if MYSQL_VERSION_ID >= 50700
	init_sql_alloc(key_memory_sql_pattern, &mem_audit, TABLE_ALLOC_BLOCK_SIZE, 0);
#else 
	init_sql_alloc(&mem_audit, ACL_ALLOC_BLOCK_SIZE, 0);
#endif 
  /* init the mutex */
  if (mysql_rwlock_init(key_rwlock_THR_LOCK_sql_pattern, &THR_LOCK_sql_pattern))
		return true; 

  /* initialise our servers cache */ //the name is pk on sql_pattern, so we make name2pattern_cache unique hash.
#if MYSQL_VERSION_ID <= 50700
  if (my_hash_init(&name2pattern_cache, system_charset_info, 512, 0, 0,
                   (my_hash_get_key) name2pattern_cache_get_key, 0, HASH_UNIQUE))
#else
  if (my_hash_init(&name2pattern_cache, system_charset_info, 512, 0, 0,
                   (my_hash_get_key) name2pattern_cache_get_key, 0, HASH_UNIQUE,
                   key_memory_sql_pattern))
#endif 
  {
    return_val= TRUE; /* we failed, out of memory? */
    goto end;
  }

  /* initialise our servers cache */
#if MYSQL_VERSION_ID <= 50700
  if (my_hash_init(&pattern2name_cache, system_charset_info, 512, 0, 0,
                   (my_hash_get_key) pattern2name_cache_get_key, 0, 0))
#else
  if (my_hash_init(&pattern2name_cache, system_charset_info, 512, 0, 0,
                   (my_hash_get_key) pattern2name_cache_get_key, 0, 0,
                   key_memory_sql_pattern))
#endif 
  {
    return_val= TRUE; /* we failed, out of memory? */
    goto end;
  }

  if (dont_read_table)
    goto end;	

end:
	return return_val;
}
bool patternCacheFree ()
{
  for (size_t index =0; index < name2pattern_cache.records; index++) {
    Name2Pattern* name2pattern= (Name2Pattern*)my_hash_element (&name2pattern_cache, index);
    if (name2pattern) {
        if (name2pattern->pattern_name && strlen(name2pattern->pattern_name)) 
            free(name2pattern->pattern_name);
        if (name2pattern->sql_pattern_str && strlen (name2pattern->sql_pattern_str))
            free(name2pattern->sql_pattern_str);
        if (name2pattern->md5Str && strlen(name2pattern->md5Str)) 
            free(name2pattern->md5Str) ;
        if (name2pattern->sql_action && strlen(name2pattern->sql_action))
            free(name2pattern->sql_action) ;

        delete name2pattern; 
        name2pattern = NULL; 
    }
  } 
  my_hash_free(&name2pattern_cache);
  
  for (size_t index =0; index < pattern2name_cache.records; index++) {
	Pattern2Name* pattern2name = (Pattern2Name*)my_hash_element (&pattern2name_cache, index);
	if (pattern2name) {
		if (pattern2name->sql_pattern_str && strlen(pattern2name->sql_pattern_str))
			free(pattern2name->sql_pattern_str) ;
		if (pattern2name->pattern_name && strlen(pattern2name->pattern_name))
			free(pattern2name->pattern_name) ;
		delete pattern2name; 
		pattern2name = NULL; 
	}
  }
  my_hash_free(&pattern2name_cache);

  return false;
}
bool sql_pattern_param_deinit (bool end)
{
  if (!my_hash_inited(&pattern2name_cache) && ! my_hash_inited(&name2pattern_cache)) //
    return (false);	
  if (!end)
  {
	my_hash_reset(&name2pattern_cache);
	my_hash_reset(&pattern2name_cache);

    free_root(&mem_audit, MYF(MY_MARK_BLOCKS_FREE));
	return (false);
  }

  patternCacheFree ();
  mysql_rwlock_destroy(&THR_LOCK_sql_pattern);
  free_root(&mem_audit,MYF(0)) ;

  return (false);
}
//describs the fields of system table sql_pattern.
enum enum_sql_pattern_table_field
{
  SQLPATTERN_FIELD_PATTERN_NAME= 0,
  SQLPATTERN_FIELD_PATTERN,
  SQLPATTERN_FIELD_MD5,
  SQLPATTERN_FIELD_SQLACTION
};

static bool
get_pattern_from_table_to_cache(THD* thd, TABLE* table)
{
  /* alloc a pattern2name and name2pattern struct */
  char *ptr;
  char * const blank= (char*)"";
	
  //table->use_all_columns();

  /* get each field into the server struct ptr */
#if MYSQL_VERSION_ID >= 50700
  table->mark_column_used (thd,table->field[SQLPATTERN_FIELD_PATTERN_NAME], MARK_COLUMNS_READ);
#endif 
  ptr= get_field(&mem_audit, table->field[SQLPATTERN_FIELD_PATTERN_NAME]);

  //if we can not get data from catalog, then returen.	
  if (!ptr || strlen(ptr)==0) 
  	return TRUE ; 

  Name2Pattern* name2pattern = new Name2Pattern ();
  Pattern2Name* pattern2name = new Pattern2Name (); 

  name2pattern->pattern_name = ptr ? strdup (ptr) : blank;
  name2pattern->pattern_name_len = strlen (name2pattern->pattern_name); //if we use same pointer to patern_name, in hash,it will be changed by splitting.
  pattern2name->pattern_name = strdup( name2pattern->pattern_name );  
	
#if MYSQL_VERSION_ID >= 50700
  table->mark_column_used (thd,table->field[SQLPATTERN_FIELD_PATTERN], MARK_COLUMNS_READ);
#endif 
  ptr= get_field(&mem_audit, table->field[SQLPATTERN_FIELD_PATTERN]);

  name2pattern->sql_pattern_str = ptr ? strdup(ptr) : blank;
  pattern2name->sql_pattern_str = strdup (name2pattern->sql_pattern_str) ;
  pattern2name->sql_pattern_str_len = strlen (pattern2name->sql_pattern_str) ;

#if MYSQL_VERSION_ID >= 50700
  table->mark_column_used (thd,table->field[SQLPATTERN_FIELD_MD5], MARK_COLUMNS_READ);
#endif 
  ptr= get_field(&mem_audit, table->field[SQLPATTERN_FIELD_MD5]);
  name2pattern->md5Str  = ptr ? strdup(ptr) : blank;

#if MYSQL_VERSION_ID >= 50700
  table->mark_column_used (thd,table->field[SQLPATTERN_FIELD_SQLACTION], MARK_COLUMNS_READ);
#endif 
  ptr= get_field(&mem_audit, table->field[SQLPATTERN_FIELD_SQLACTION]);
  name2pattern->sql_action = ptr ? strdup(ptr) : blank;

  mysql_rwlock_wrlock(&THR_LOCK_sql_pattern);

  //pattern_name is pk on sysmtem table sql_pattern, so that there is no dup entry in name2pattern_cache.
  if (my_hash_insert(&name2pattern_cache, (uchar*) name2pattern))
  {
    // error handling needed here
    mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
    return (TRUE);
  }

  if (my_hash_insert(&pattern2name_cache, (uchar*) pattern2name))
  {
    // error handling needed here
	 mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
    return (TRUE);
  }  
	
  mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
  return (FALSE);
}


static bool
sql_pattern_load(THD* thd, TABLE* table)
{
  READ_RECORD read_record_info;
  bool return_val= TRUE;

  if (init_read_record(&read_record_info, thd, table,
                       NULL, 1, 1, FALSE))
    return (TRUE);

  table->use_all_columns();
  while (!(read_record_info.read_record(&read_record_info)))
  {
    /* return_val is already TRUE, so no need to set */
    if ((get_pattern_from_table_to_cache(thd, table)))
      goto end;
  }

  return_val= FALSE;

end:
  end_read_record(&read_record_info);
  return (return_val);
}

bool load_sql_pattern(THD* thd)
{
  TABLE_LIST tables[1];
  bool return_val= true;

  tables[0].init_one_table("mysql", 5, "sql_pattern", 11, "sql_pattern", TL_READ);
#if MYSQL_VERSION_ID <= 50700
  if (open_normal_and_derived_tables(thd, tables, 0)){
     /*
       Execution might have been interrupted; only print the error message
       if an error condition has been raised.
     */
     return_val = false ;
     goto end ;
  }
#else
  if (open_trans_system_tables_for_read(thd, tables))
  {
    /*
      Execution might have been interrupted; only print the error message
      if an error condition has been raised.
    */
	return_val = false ;
    goto end;
  }
#endif 

  if ((return_val= sql_pattern_load(thd, tables[0].table)))
  {                 // Error. Revert to old list
    /* blast, for now, we have no servers, discuss later way to preserve */
  }

end:
#if MYSQL_VERSION_ID <=50700
  trans_commit_stmt(thd);
  close_mysql_tables(thd);
#else
  close_trans_system_tables(thd);
#endif
  return (return_val);
}

//for performance consideration, the we use loop to retrive the data, not hash_search, it makes a lot of function calls.
Pattern2Name* getPatterNameByPattern (const char* pattern) 
{
	Pattern2Name* pattern2name = NULL; 

    mysql_rwlock_rdlock(&THR_LOCK_sql_pattern);
	pattern2name = (Pattern2Name*) my_hash_search (&pattern2name_cache, (uchar*) pattern, strlen(pattern)) ;	
   	mysql_rwlock_unlock(&THR_LOCK_sql_pattern);

	return pattern2name; 
}

Pattern2Name* getPatterNameByPatternNext (const char* pattern) 
{
	Pattern2Name* pattern2name = NULL; 
	static HASH_SEARCH_STATE state;

    mysql_rwlock_rdlock(&THR_LOCK_sql_pattern);
	pattern2name = (Pattern2Name*) my_hash_next (&pattern2name_cache, (uchar*) pattern, strlen(pattern), &state) ;	
   	mysql_rwlock_unlock(&THR_LOCK_sql_pattern);

	return pattern2name; 
}

Name2Pattern* getPatternByPatternName ( const char* patternName)
{
    Name2Pattern* name2pattern= NULL;

    mysql_rwlock_rdlock(&THR_LOCK_sql_pattern);
	name2pattern = (Name2Pattern*) my_hash_search (&name2pattern_cache, (uchar*) patternName, strlen(patternName)) ;
    mysql_rwlock_unlock(&THR_LOCK_sql_pattern);

    return name2pattern;
}

bool deleteElemByPattern(const char* pattern)
{
	bool ret (false) ;
    Pattern2Name* pattern2name = NULL;
    mysql_rwlock_wrlock(&THR_LOCK_sql_pattern);

    pattern2name = (Pattern2Name*) my_hash_search (&pattern2name_cache, (uchar*) pattern, strlen(pattern)) ;
	if (pattern2name) {
		ret = my_hash_delete ( &pattern2name_cache, (uchar*) pattern2name);

		if (pattern2name->sql_pattern_str && strlen(pattern2name->sql_pattern_str))
			free (pattern2name->sql_pattern_str);
		if (pattern2name->pattern_name && strlen(pattern2name->pattern_name))
			free (pattern2name->pattern_name) ;

		delete pattern2name; 
		pattern2name = NULL ;
	}
	else {
    	mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
		return true; 	
	}	
   	mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
	return ret; 
}

bool deleteElemByPattern(const char* patternName, const char* pattern)
{//to delete the elem by pattern and pattername by looping the hash elems.
	bool ret (false) ;
    Pattern2Name* pattern2name = NULL;

    mysql_rwlock_wrlock(&THR_LOCK_sql_pattern); //the pattern is not prk,so we can have mult same pattern with diff names.
	for (size_t idx =0; idx < pattern2name_cache.records; idx++) {
		pattern2name = (Pattern2Name*) my_hash_element (&pattern2name_cache, idx) ;
    	if (pattern2name && !strcmp (patternName, pattern2name->pattern_name)) {
        	ret = my_hash_delete ( &pattern2name_cache, (uchar*) pattern2name);

	        if (pattern2name->sql_pattern_str && strlen(pattern2name->sql_pattern_str))
    	        free (pattern2name->sql_pattern_str);
        	if (pattern2name->pattern_name && strlen(pattern2name->pattern_name))
            	free (pattern2name->pattern_name) ;

	        delete pattern2name;
    	    pattern2name = NULL ;
			
			break; 
    	}
	}

   	mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
	return ret; 
}

bool deleteElemByPatternName (const char* patternName)
{
	bool ret (false);
    Name2Pattern* name2pattern= NULL;
    mysql_rwlock_wrlock(&THR_LOCK_sql_pattern);
    name2pattern = (Name2Pattern*) my_hash_search (&name2pattern_cache, (uchar*) patternName, strlen(patternName)) ;
	
	if (name2pattern) {
		ret = my_hash_delete (&name2pattern_cache, (uchar*) name2pattern);

		if (name2pattern->pattern_name && strlen(name2pattern->pattern_name)) 
			free(name2pattern->pattern_name);
		if (name2pattern->sql_pattern_str && strlen(name2pattern->sql_pattern_str))
			free(name2pattern->sql_pattern_str);
		if (name2pattern->md5Str && strlen(name2pattern->md5Str)) 
			free(name2pattern->md5Str) ;
		if (name2pattern->sql_action && strlen(name2pattern->sql_action))
			free(name2pattern->sql_action) ;

		delete name2pattern; 
		name2pattern = NULL; 
	}	
	else {
    	mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
		return true; 
	}
   	mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
	return ret ; 
}

bool insertElemName2PatternCache (const char* patternName, const char* pattern_str, const char* md5Str, const char* actionStr) 
{
	//pattern name is primary key, so we check it.
	Name2Pattern* exists = getPatternByPatternName ( patternName);
	if (exists) {
		deleteElemByPatternName ( patternName);
	}
	Name2Pattern* name2pattern = new Name2Pattern (); //patternName will be free in sql->release().therefore we keep a copy of these.
	name2pattern->pattern_name = strdup ((char*)patternName); 
	name2pattern->pattern_name_len = strlen (patternName);
	name2pattern->sql_pattern_str = strdup ((char*)pattern_str); 
	name2pattern->md5Str = strdup((char*)md5Str); 
	name2pattern->sql_action = strdup ((char*)actionStr); 

    mysql_rwlock_wrlock(&THR_LOCK_sql_pattern);
	bool ret =  my_hash_insert (&name2pattern_cache, (uchar*) name2pattern); 	
	if (ret)
		sql_print_error("can not insert elem into name2pattern_cache, the patternName:%s, pattern:%s, md5:%s, action:%s", \
									patternName, pattern_str, md5Str, actionStr);
    mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
	
	return ret; 
}

bool insertElemPattern2NameCache (const char* sql_pattern, const char* patternName) 
{ //there is some memory leakage in this code, but is not a matter, it's tiny things, because the patterns are not a larg part.
	Pattern2Name* pattern2name = new Pattern2Name ();
	pattern2name->sql_pattern_str = strdup ((char*)sql_pattern);
	pattern2name->sql_pattern_str_len = strlen (pattern2name->sql_pattern_str);
	pattern2name->pattern_name = strdup((char*)patternName); 

    mysql_rwlock_wrlock(&THR_LOCK_sql_pattern);
	bool ret =  my_hash_insert (&pattern2name_cache, (uchar*) pattern2name);
	if (ret) 
		sql_print_error("can not insert elem into pattern2name_cache, the patternName:%s, pattern:%s", patternName, sql_pattern);
    mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
	return ret; 
}

bool updateElemName2PatternCache (const char* patternName, const char* pattern_str, const char* md5Str, const char* actionStr)
{
    bool ret (false);
    Name2Pattern* name2pattern = NULL;
    mysql_rwlock_wrlock(&THR_LOCK_sql_pattern);
    name2pattern = (Name2Pattern*) my_hash_search (&name2pattern_cache, (uchar*) patternName, strlen(patternName)) ;

    if (name2pattern) {
        ret = my_hash_delete (&name2pattern_cache, (uchar*) name2pattern);
		if (ret)
			sql_print_error("[updateElemName2PatternCache] can not delete elem from name2pattern_cache, the patternName:%s, pattern:%s ", patternName, pattern_str);

        if (name2pattern->pattern_name && strlen (name2pattern->pattern_name)) 
            free(name2pattern->pattern_name);
        if (name2pattern->sql_pattern_str && strlen(name2pattern->sql_pattern_str))
            free(name2pattern->sql_pattern_str);
        if (name2pattern->md5Str && strlen(name2pattern->md5Str)) 
            free(name2pattern->md5Str) ;
        if (name2pattern->sql_action && strlen(name2pattern->sql_action))
            free(name2pattern->sql_action) ;			
        delete name2pattern;
        name2pattern = NULL;

		name2pattern = new Name2Pattern  ();
		name2pattern ->pattern_name = strdup ((char*) patternName); 
		name2pattern ->pattern_name_len = strlen(patternName) ;
		name2pattern ->sql_pattern_str= strdup((char*) pattern_str); 
		name2pattern ->md5Str = strdup((char*) md5Str); 
		name2pattern ->sql_action = strdup((char*) actionStr); 
		
		ret =  my_hash_insert (&name2pattern_cache, (uchar*) name2pattern);
		if (ret)
			sql_print_error("[updateElemName2PatternCache] can not insert elem into name2pattern_cache, the patternName:%s, pattern:%s ", patternName, pattern_str);
    }else  
        ret = true;

    mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
    return ret ;
}

bool updateElemPattern2NameCache (const char* sql_pattern, const char* patternName)
{
    bool ret (false) ;
    Pattern2Name* pattern2name = NULL;
    mysql_rwlock_wrlock(&THR_LOCK_sql_pattern);

    pattern2name = (Pattern2Name*) my_hash_search (&pattern2name_cache, (uchar*) sql_pattern, strlen(sql_pattern)) ;
    if (pattern2name) {
        ret = my_hash_delete ( &pattern2name_cache, (uchar*) pattern2name);
		if (ret)
			sql_print_error("[updateElemPattern2NameCache]can not delete elem from pattern2name_cache, the patternName:%s, pattern:%s ", patternName, sql_pattern);

		if (pattern2name->sql_pattern_str && strlen(pattern2name->sql_pattern_str)) //this is allocated by 'strdup' when do insertion.
			free (pattern2name->sql_pattern_str) ;
		if (pattern2name->pattern_name && strlen(pattern2name->pattern_name))
			free (pattern2name->pattern_name) ;
        delete pattern2name;
        pattern2name = NULL ;

		pattern2name = new Pattern2Name (); //insert new one.
		pattern2name->sql_pattern_str = strdup ((char*) sql_pattern); 
		pattern2name->sql_pattern_str_len = strlen (pattern2name->sql_pattern_str);
		pattern2name->pattern_name = strdup((char*) patternName); 

		ret =  my_hash_insert (&pattern2name_cache, (uchar*) pattern2name);
		if (ret) 
			sql_print_error("[updateElemPattern2NameCache]can not insert elem into pattern2name_cache, the patternName:%s, pattern:%s ", patternName, sql_pattern);
		delete pattern2name;
			
    } else 
        ret = true;

    mysql_rwlock_unlock(&THR_LOCK_sql_pattern);
	return ret; 
}
