#ifndef __SQL_PATTERN_H__
#define __SQL_PATTERN_H__

#include "table.h"
#include "records.h"
#include "keycache.h"

#if MYSQL_VERSION_ID <= 50700
extern bool open_normal_and_derived_tables(THD *thd, TABLE_LIST *table_list, uint flags);
extern void close_mysql_tables(THD *thd);
extern bool trans_commit_stmt(THD *thd) ;
#else 
extern bool open_trans_system_tables_for_read(THD *thd, TABLE_LIST *table_list);
extern void close_trans_system_tables(THD *thd) ;
#endif 

class THD;
struct TABLE;
typedef struct st_mem_root MEM_ROOT;


//In this hash table. we store the mapping relationship: sql_pattern_name <---> sql_pattern [key: sql_pattern_name]
//This hash table used by 'create/update/drop rules xxxx'. but in mysql_parse function, we only get a sql pattern not
//pattern_name, therfore we need another hash to store the relationship between sql pattern and pattern name. Consider that
//function calc_md5 will consume a mount of cpu cycles, so we don not use md5 as hash key, but using sql pattern as the hash key.

//extern MEM_ROOT mem_audit;
//extern PSI_memory_key key_memory_sql_pattern ;
//static mysql_rwlock_t THR_LOCK_sql_pattern;

//For name 2 pattern mapping relationship.
class Name2Pattern{
public:
	Name2Pattern(): pattern_name(NULL), pattern_name_len(0), sql_pattern_str(NULL), md5Str(NULL), sql_action(NULL) {
	}

    char* pattern_name;  
	size_t pattern_name_len;
	char *sql_pattern_str, *md5Str, *sql_action;
} ;

class Pattern2Name {
public:
	Pattern2Name (): sql_pattern_str (NULL), sql_pattern_str_len(0), pattern_name(NULL) {
	} 

	char* sql_pattern_str ; 
	size_t sql_pattern_str_len ; 
	char* pattern_name;
} ;

bool load_sql_pattern(THD* thd) ;
bool sql_pattern_param_init (bool dont_read_table);
bool sql_pattern_param_deinit (bool dont_read_table);
bool isMapInitialized () ;
Pattern2Name* getPatterNameByPattern (const char* pattern) ;
Pattern2Name* getPatterNameByPatternNext (const char* pattern) ;
Name2Pattern* getPatternByPatternName(const char* patternName);

bool deleteElemByPattern(const char* pattern);
bool deleteElemByPattern(const char* patterName, const char* pattern);
bool deleteElemByPatternName (const char* patternName);

bool insertElemPattern2NameCache (const char* sql_pattern, const char* patternName) ;
bool insertElemName2PatternCache (const char* patternName, const char* pattern_str, const char* md5Str, const char* actionStr) ;

bool updateElemPattern2NameCache (const char* sql_pattern, const char* patternName) ;
bool updateElemName2PatternCache (const char* patternName, const char* pattern_str, const char* md5Str, const char* actionStr) ;

bool patternCacheFree () ;

#endif 
