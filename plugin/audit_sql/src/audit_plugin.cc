 /*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#include "mysql_inc.h"
#include <mysql_version.h>

#if MYSQL_VERSION_ID <= 50700
#include "my_global.h"
#include "sql_rewrite.h"
#else
#include "sql_query_rewrite.h"
#endif 

#include "my_dbug.h"
#include "sp_cache.h"

#if MYSQL_VERSION_ID >= 50700 
#include "conn_handler/connection_handler_manager.h"
#endif 

#include "sql_pattern.h"
#include "hot_patch.h"
#include <stdlib.h>
#include <ctype.h>
#include <pwd.h>
#include <parser.h>
#include <nodes.h>

#include "audit_handler.h"
#include <string.h>
#include <sys/mman.h>
#if MYSQL_VERSION_ID >= 50600
// in 5.6 md5 implementation changed and we include our own
#include "md5.h"
#endif

#if defined(__GET_TEXT__)
#include <libintl.h>
#define get_text(T) gettext(T)
#else
#define get_text(T) (T)
#endif 

/*
 Disable __attribute__() on non-gcc compilers.

 #if !defined(__attribute__) && !defined(__GNUC__)
 #define __attribute__(A)
 #endif
*/

//all this vars are used to slient the linker. this var are used in mysqld under debug mode. 
//when we are in release mode, the plugin can not find these vars, so we add some dummy var to cheat linker.
//the marco DBUG_OFF MUST BE SAME AS mysqld, which means the compiling options 'debug mode' MUST be same as mysqlds'.
#if MYSQL_VERSION_ID <= 50700
	#ifndef DBUG_OFF
	struct CODE_STATE ;
	my_bool _dbug_on_ = 0;
    int  _db_enabled_() {return 0;} 
	void _db_pargs_(uint _line_, const char *keyword) {}
	void _db_doprnt_(const char *format,...) {}
	void _db_enter_(const char *_func_, const char *_file_,
                uint _line_, struct _db_stack_frame_ *_stack_frame_) {}
	void _db_return_(uint _line_, struct _db_stack_frame_ *_stack_frame_){}
	my_bool _db_keyword_(_db_code_state_*cs, const char *keyword, int strict) {return false;}
	#else //to cheat the compiler. we do not use these functions under release mode.
    my_bool _dbug_on_ = 0 ;
    int _db_enabled_ =0;
    int _db_pargs_ = 0;
    int _db_doprnt_ = 0;
    int _db_enter_ =0;
    int _db_return_ =0;
	int _db_keyword_ = 0;
	#endif
#else 
#ifdef DBUG_OFF 
	int _dbug_on_ = 0 ;
	//int _db_enabled_() {return 0;}
	int _db_enabled_ =0; 
	int _db_pargs_ = 0;
	int _db_doprnt_ = 0;
	int _db_enter_ =0;
	int _db_return_ =0;
#endif
#endif 

static const unsigned int _2GB_ = 2147483648 ; //2GB.
static const char *log_prefix = AUDIT_LOG_PREFIX;
static const char *audit_plugin_dot_str = "."; 
static const char *audit_plugin_stardot_str = "*.";
static const char *audit_plugin_dotstar_str = ".*";

static const char *audit_plugin_com_str = "Com";
static const char *audit_plugin_conn_str = "Connect";
static const char *audit_plugin_quit_str = "Quit";
static const char *audit_plugin_failed_str = "Failed Login";
static const char *audit_plugin_show_fields_str = "show_fields";
#if MYSQL_VERSION_ID < 50600
static const char *audit_plugin_event_sch_str ="event_scheduler";
#endif 
static const char *audit_plugin_select_str= "select";
static const char *audit_plugin_prepare_sql_str="prepare_sql";

// possible audit handlers
static Audit_file_handler json_file_handler;
static Audit_socket_handler json_socket_handler;

// formatters
static Audit_json_formatter json_formatter;

// flags to hold if audit handlers are enabled
static my_bool json_file_handler_enable = FALSE;
static my_bool force_record_logins_enable = FALSE;
static my_bool json_file_handler_flush = FALSE;
static my_bool json_socket_handler_enable = FALSE;
static my_bool uninstall_plugin_enable = FALSE;
static my_bool validate_checksum_enable = FALSE;
static my_bool offsets_by_version_enable = FALSE;
static my_bool validate_offsets_extended_enable = FALSE;
static my_bool parsing_sql_enable = FALSE; 

static char *offsets_string = NULL;
static char *checksum_string = NULL;
static int delay_ms_val = 0;
static char *delay_cmds_string = NULL;
static char delay_cmds_buff[4096] = {0};
static char *whitelist_cmds_string = NULL;
static char whitelist_cmds_buff[4096] = {0};
static char *record_cmds_string = NULL;
static char record_cmds_buff[4096] = {0};
static char *password_masking_cmds_string = NULL;
static char password_masking_cmds_buff[4096] = {0};
static char *record_objs_string = NULL;
static char record_objs_buff[4096] = {0};
static char *whitelist_users_string = NULL;
static char whitelist_users_buff[4096] = {0};
#if defined (USING_IP_AND_PORT)
static char remote_ip_buffer[17]={"127.0.0.1"} ;
#endif 
//static char *remote_ip= NULL;
static char socket_type_buffer[5]={"unix"};
//static char *socket_type =NULL ;
static char dns_svr_addr_buffer[1024]={0} ; //the dns svr addr.

typedef enum {
	FUNC_AUDIT =0 ,
	FUNC_FIREWALL, 
	FUNC_BOTH 
} FUNCTION_TYPES ; 
static char* function_type_string =NULL;
static char  function_type_buffer [16] = {"audit"};
static int   function_type = FUNC_AUDIT; 

static char delay_cmds_array [SQLCOM_END + 2][MAX_COMMAND_CHAR_NUMBERS] = {{0}};
static char whitelist_cmds_array [SQLCOM_END + 2][MAX_COMMAND_CHAR_NUMBERS] = {{0}};
static char record_cmds_array [SQLCOM_END + 2][MAX_COMMAND_CHAR_NUMBERS] = {{0}};
static char password_masking_cmds_array [SQLCOM_END + 2][MAX_COMMAND_CHAR_NUMBERS] = {{0}};
static char record_objs_array [MAX_NUM_OBJECT_ELEM + 2][MAX_OBJECT_CHAR_NUMBERS] = {{0}};
static char whitelist_users_array [MAX_NUM_USER_ELEM + 2][MAX_USER_CHAR_NUMBERS] = {{0}};
static bool record_empty_objs_set = true;
static int num_delay_cmds = 0;
static int num_whitelist_cmds = 0;
static int num_record_cmds = 0;
static int num_password_masking_cmds = 0;
static int num_record_objs = 0;
static int num_whitelist_users = 0;
static unsigned long  max_seg_log_file_size = _2GB_; //2G, 2*1024*1024*1024.
#if defined (USING_IP_AND_PORT)
#endif
static int remote_port= 5000;

static SHOW_VAR com_status_vars_array [MAX_COM_STATUS_VARS_RECORDS] = {{0}};

enum before_after_enum {
	AUDIT_AFTER = 0,	// default
	AUDIT_BEFORE,
	AUDIT_BOTH
};

static ulong before_after_mode = AUDIT_AFTER;

// regex stuff
static char password_masking_regex_check_buff[4096] = {0};
static char * password_masking_regex_string = NULL;
static char password_masking_regex_buff[4096] = {0};

#define _COMMENT_SPACE_ "(?:/\\*.*?\\*/|\\s)*?"
#define _QUOTED_PSW_ "[\'|\"](?<psw>.*?)(?<!\\\\)[\'|\"]"

static const char default_pw_masking_regex[] =
	// identified by [password] '***'
	"identified"_COMMENT_SPACE_"by"_COMMENT_SPACE_"(?:password)?"_COMMENT_SPACE_ _QUOTED_PSW_
	// password function
	"|password"_COMMENT_SPACE_"\\("_COMMENT_SPACE_ _QUOTED_PSW_ _COMMENT_SPACE_"\\)"
	// Used at: CHANGE MASTER TO MASTER_PASSWORD='new3cret', SET PASSWORD [FOR user] = 'hash', password 'user_pass';
	"|password"_COMMENT_SPACE_"(?:for"_COMMENT_SPACE_"\\S+?)?"_COMMENT_SPACE_"="_COMMENT_SPACE_ _QUOTED_PSW_
	"|password"_COMMENT_SPACE_ _QUOTED_PSW_
	// federated engine create table with connection. See: http://dev.mysql.com/doc/refman/5.5/en/federated-create-connection.html
	// commented out as federated engine is disabled by default
	// "|ENGINE"_COMMENT_SPACE_"="_COMMENT_SPACE_"FEDERATED"_COMMENT_SPACE_".*CONNECTION"_COMMENT_SPACE_"="_COMMENT_SPACE_"[\'|\"]\\S+?://\\S+?:(?<psw>.*)@\\S+[\'|\"]"
	;

// socket name
static char json_socket_name_buff[1024] = {0};

// Define default port in case user configured out port and socket in my.cnf (bug 1151389)
#define MYSQL_DEFAULT_PORT 3306

// Default value for write timeout in microseconds
#define DEFAULT_WRITE_TIMEOUT 1000 // milliseconds --> 1 second - MySQL API uses seconds

/**
 * The trampoline functions we use. Will be set to point to allocated mem.
 */
#if MYSQL_VERSION_ID <= 50700
static int (*trampoline_mysql_execute_command)(THD *thd) = NULL;
static unsigned int trampoline_mysql_execute_size = 0;
#else
static int (*trampoline_mysql_execute_command)(THD *thd, bool first_level) = NULL;
static unsigned int trampoline_mysql_execute_size = 0;
#endif

#if MYSQL_VERSION_ID <= 50700
static void (*trampoline_mysql_parse)(THD *thd, char *rawbuf, uint length,  Parser_state *parser_state) = NULL;
static unsigned int trampoline_mysql_parse_size = 0;
#else 
static void (*trampoline_mysql_parse)(THD *thd, Parser_state *parser_state) = NULL;
static unsigned int trampoline_mysql_parse_size = 0;
#endif 

#if MYSQL_VERSION_ID < 50600
static void (*trampoline_log_slow_statement)(THD *thd) = NULL;
static unsigned int trampoline_log_slow_statement_size = 0;
#endif

#if MYSQL_VERSION_ID < 50505
static int (*trampoline_check_user)(THD *thd, enum enum_server_command command, const char *passwd, uint passwd_len, const char *db, bool check_count) = NULL;
static unsigned int trampoline_check_user_size = 0;
#elif MYSQL_VERSION_ID < 50600
static bool (*trampoline_acl_authenticate)(THD *thd, uint connect_errors, uint com_change_user_pkt_len) = NULL;
static unsigned int trampoline_acl_authenticate_size = 0;
#endif

static MYSQL_THDVAR_ULONG(is_thd_printed_list,
	PLUGIN_VAR_READONLY | PLUGIN_VAR_NOSYSVAR | PLUGIN_VAR_NOCMDOPT,
	get_text("avoid duplicate printing"),
	NULL, NULL,0,0,
#ifdef __x86_64__
	0xffffffffffffff,
#else
	0xffffffff,
#endif
	1);

static MYSQL_THDVAR_ULONG(query_cache_table_list,
	PLUGIN_VAR_READONLY | PLUGIN_VAR_NOSYSVAR | PLUGIN_VAR_NOCMDOPT,
	get_text("Pointer to query cache table list."),
	NULL, NULL, 0, 0,
#ifdef __x86_64__
	0xffffffffffffff,
#else
	0xffffffff,
#endif
	1);


static MYSQL_THDVAR_BOOL(set_peer_cred,
	PLUGIN_VAR_READONLY | PLUGIN_VAR_NOSYSVAR | PLUGIN_VAR_NOCMDOPT,
	get_text("track if peer credential information is set"),
	NULL, NULL, 0);

static MYSQL_THDVAR_BOOL(peer_is_uds,
	PLUGIN_VAR_READONLY | PLUGIN_VAR_NOSYSVAR | PLUGIN_VAR_NOCMDOPT,
	get_text("track if client is connected on Unix Domain Socket"),
	NULL, NULL, 0);

// We use a THDVAR_STR to store the PeerInfo struct for this thread.
// In order to get MySQL to allocate storage correctly, we have to
// fool it into thinking that the storage holds a C string. To do so,
// we create a char array of the right size and use that as the initial
// value. Then in the constructor routines, below, we initialize the
// contents to look like a C string with a bunch of '0' characters,
// terminated by a final '\0'.
char peer_info_init_value[sizeof(struct PeerInfo) + 4];

#if defined(MARIADB_BASE_VERSION)
static MYSQL_THDVAR_ULONG(peer_info,
	PLUGIN_VAR_READONLY | PLUGIN_VAR_NOSYSVAR | PLUGIN_VAR_NOCMDOPT,
	get_text("Pointer to peer info data"),
	NULL, NULL, 0, 0,
#ifdef __x86_64__
	0xffffffffffffff,
#else
	0xffffffff,
#endif
	1);
#else	// NOT mariadb
static MYSQL_THDVAR_STR(peer_info,
	PLUGIN_VAR_NOSYSVAR | PLUGIN_VAR_READONLY |
			PLUGIN_VAR_MEMALLOC,
	get_text("Info related to client process"),
	NULL, NULL, peer_info_init_value);
#endif

#define RELEASE_SQL_OBJ(obj)	if(obj){ 				\
										obj->release();	\
										delete obj;		\
										obj = NULL; 	\
									}

//Parser* m_parser = NULL; 
THDPRINTED *GetThdPrintedList(THD *thd)
{
	THDPRINTED *pThdPrintedList= (THDPRINTED*) THDVAR(thd, is_thd_printed_list);
	if (pThdPrintedList)
	{
		return pThdPrintedList;
	}
	THDVAR(thd, is_thd_printed_list) = 0;
	return NULL;
}

static void initializePeerCredentials(THD *pThd)
{
	int sock;
	struct stat sbuf;
	struct ucred cred;
	socklen_t cred_len = sizeof(cred);
	char buf[BUFSIZ];
	struct passwd pwd, *pwbuf = NULL;
	char *username = NULL;
	int fd;
	PeerInfo *peer;

#if defined(MARIADB_BASE_VERSION)
	if (THDVAR(pThd, peer_info) == 0)
	{
		peer = (PeerInfo *) malloc(sizeof(PeerInfo));
		if (peer)
		{
			memset(peer, 0, sizeof(PeerInfo));
			THDVAR(pThd, peer_info) = (ulong) peer;
		}
		else
		{
			goto done;
		}
	}
	else
	{
		peer = (PeerInfo *) THDVAR(pThd, peer_info);
	}
#else
	// zero out thread local copy of PeerInfo
	peer = (PeerInfo *) THDVAR(pThd, peer_info);
	if (peer != NULL)
	{
		memset(peer, 0, sizeof(PeerInfo));
	}
#endif

	// get the NET structure
	sock = Audit_formatter::thd_client_fd(pThd);
	// These tests are not chained so that we can add
	// debug prints if necessary.
	if (sock < 0)
	{
		goto done;
	}

	// Is it a Unix Domain socket?
	if (fstat(sock, &sbuf) < 0)
	{
		goto done;
	}

	if ((sbuf.st_mode & S_IFMT) != S_IFSOCK)
	{
		goto done;
	}

	// At this point, we know we have a Unix domain socket.
	if (! json_formatter.m_write_socket_creds)
	{
		// We need to set this, so that we don't send the
		// client port, but we don't bother getting the command
		// name and user name, since they won't be sent.
		THDVAR(pThd, peer_is_uds) = TRUE;
		goto done;
	}

	// Do a SO_PEERCRED
	if (getsockopt(sock, SOL_SOCKET, SO_PEERCRED, &cred, &cred_len) < 0)
	{
		goto done;
	}

	if (cred_len != sizeof(cred))
	{
		goto done;
	}

	if (peer == NULL)
	{
		goto done;
	}

	// Set PID
	peer->pid = cred.pid;

	// Get OS user name
	getpwuid_r(cred.uid, & pwd, buf, sizeof(buf), & pwbuf);
	if (pwbuf == NULL)
	{
		// no name, send UID
		snprintf(buf, sizeof(buf) - 1, "%lu", (unsigned long) cred.uid);
		username = buf;
	}
	else
	{
		username = pwd.pw_name;
	}

	strncpy(peer->osUser, username, PeerInfo::MAX_USER_NAME_LEN);
	peer->osUser[PeerInfo::MAX_USER_NAME_LEN] = '\0';

	// Get the progran name out of /proc
	snprintf(buf, sizeof(buf) - 1, "/proc/%d/cmdline", cred.pid);
	fd = open(buf, O_RDONLY);
	if (fd >= 0)
	{
		char data[PATH_MAX+1]={0};

		ssize_t count = read(fd, data, sizeof(data) - 1);
		if (count > 0)
		{
			data[count] = '\0';	// just in case

			if (strlen(data) <= PeerInfo::MAX_APP_NAME_LEN)
			{
				strncpy(peer->appName, data, strlen(data));
			}
			else
			{
				// take last 128 characters, typically will be
				// .../x/y/appname
				char *cp = data + strlen(data) - PeerInfo::MAX_APP_NAME_LEN;

				strncpy(peer->appName, cp, PeerInfo::MAX_APP_NAME_LEN);
				peer->appName[PeerInfo::MAX_APP_NAME_LEN] = '\0';
			}
		}

		close(fd);
	}

	// in case of errors:
	if (peer->appName[0] == '\0')
	{
		snprintf(peer->appName, sizeof(peer->appName) - 1, "pid:%d", cred.pid);
	}

	// set that we have a UDS, so that THD vars will be used
	THDVAR(pThd, peer_is_uds) = TRUE;

done:
	THDVAR(pThd, set_peer_cred) = TRUE;
}

PeerInfo *retrieve_peerinfo(THD *thd)
{
	if (! THDVAR(thd, set_peer_cred))
	{
		initializePeerCredentials(thd);
	}

	if (json_formatter.m_write_socket_creds)
	{
		PeerInfo *peer = (PeerInfo *) THDVAR(thd, peer_info);

		if (THDVAR(thd, peer_is_uds) && peer != NULL);
		{
			return peer;
		}
	}

	return NULL;
}
// cmds[] is a list of "commands" (names, commands, whatever) to
// check against the list stored in `array'.  Although declared
// here as simple `char *', the `array' is actually a two-dimensional
// array where each element is `length' bytes long. (An array of
// strings.)
//
// We loop over the array and for each element see if it's also
// in `cmds'.  If so, we return 1, otherwise we return 0.
//
// This should really be of type bool and return true/false.
static int check_array(const char *cmds[],const char *array, int length)
{
    for (int k = 0; array[k * length] !='\0';k++)
    {
        const char *elem = array + (k * length);
        for (int q = 0; cmds[q] != NULL; q++)
        {
            const char *cmd = cmds[q];
            if (strcasecmp(cmd, elem) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

// utility method checks if the passed db object is part of record_objs_array
static bool check_db_obj(const char *db, const char *name)
{
	char db_obj[MAX_OBJECT_CHAR_NUMBERS] = {0};
	char wildcard_obj[MAX_OBJECT_CHAR_NUMBERS] = {0};
	char db_wildcard[MAX_OBJECT_CHAR_NUMBERS] = {0};
	if (db && name &&
		((strlen(db) + strlen(name)) < MAX_OBJECT_CHAR_NUMBERS - 2))
	{
		strncpy(db_obj, db, strlen(db));
		strncat(db_obj, audit_plugin_dot_str, strlen(audit_plugin_dot_str));
		strncat(db_obj, name, strlen(name));
		strncpy(wildcard_obj, audit_plugin_stardot_str, strlen(audit_plugin_stardot_str));
		strncat(wildcard_obj, name, strlen(name));
		strncpy(db_wildcard, db, strlen(db));
		strncat(db_wildcard, audit_plugin_dotstar_str, strlen(audit_plugin_dotstar_str));
		const char *objects[4]={0};
		objects[0] = db_obj;
		objects[1] = wildcard_obj;
		objects[2] = db_wildcard;
		objects[3] = NULL;
		return check_array(objects, static_cast<char *> (record_objs_array[0]), MAX_OBJECT_CHAR_NUMBERS);
	}
	return false;
}

// callback function returns if password masking is required according to cmd type
static my_bool check_do_password_masking(const char * cmd)
{
	if (num_password_masking_cmds > 0)
	{
		const char *cmds[2]={0};
		cmds[0] = cmd;
		cmds[1] = NULL;
		return check_array(cmds, static_cast<const char *> (password_masking_cmds_array[0]), sizeof(password_masking_cmds_array[0]));
	}
	return false;
}

static void do_delay(ThdSesData *pThdData)
{
	if (delay_ms_val > 0)
	{
		const char *cmds[2]={0};

		const char *cmd = pThdData->getCmdName();
		cmds[0] = cmd;
		cmds[1] = NULL;
		int delay = check_array(cmds, static_cast<char *> (delay_cmds_array[0]), MAX_COMMAND_CHAR_NUMBERS);
		if (delay)
		{
			// Audit_file_handler::print_sleep(thd,delay_ms_val);
			my_sleep(delay_ms_val *1000);
		}
	}
}

static void audit(ThdSesData *pThdData)
{
	THDPRINTED *pThdPrintedList = GetThdPrintedList(pThdData->getTHD());

	if (num_whitelist_cmds > 0)
	{
		const char * cmd = pThdData->getCmdName();
		const char *cmds[2]={0};
		cmds[0] = cmd;
		cmds[1] = NULL;
		if (check_array(cmds, static_cast<char *> (whitelist_cmds_array[0]), MAX_COMMAND_CHAR_NUMBERS))
		{
			return;
		}
	}

	if (num_whitelist_users > 0)
	{
		const char *user = pThdData->getUserName(); // If name is present, then no need to log the query
		const char *users[2]={0};
		if (NULL == user || '\0' == user[0]) // empty user use special symbol: "{}"
		{
			user = "{}";
		}
		users[0] = user;
		users[1] = NULL;
		if (check_array(users, static_cast<char *> (whitelist_users_array[0]), MAX_USER_CHAR_NUMBERS))
		{
			return;
		}
	}

	bool do_objs_cmds_check = true;
	if (force_record_logins_enable)
	{
		const char *cmd = pThdData->getCmdName();
		if (strcasecmp(cmd, audit_plugin_conn_str) == 0
			|| strcasecmp(cmd, audit_plugin_quit_str) == 0
			|| strcasecmp(cmd, audit_plugin_failed_str) == 0)
		{
			do_objs_cmds_check = false;
		}
	}

	if (num_record_cmds > 0 && do_objs_cmds_check)
	{
		const char *cmd = pThdData->getCmdName();
		const char *cmds[2]={0};
		cmds[0] = cmd;
		cmds[1] = NULL;
		
		if (! check_array(cmds, static_cast<char *>(record_cmds_array[0]), MAX_COMMAND_CHAR_NUMBERS))
		{
			return;
		}
	}

	if (num_record_objs > 0 && do_objs_cmds_check)
	{
		bool matched = false;
		if (pThdData->startGetObjects())
		{
			const char *db_name = NULL;
			const char *obj_name = NULL;
			while(!matched && pThdData->getNextObject(&db_name, &obj_name, NULL))
			{
				matched = check_db_obj(db_name, obj_name);
			}
		}
		else // no objects
		{
			matched = record_empty_objs_set;
		}
		if (! matched)
		{
			return;
		}
	}

	if (before_after_mode != AUDIT_BOTH && pThdPrintedList && pThdPrintedList->cur_index < MAX_NUM_QUEUE_ELEM)
	{
		// audit the event if we haven't done so yet or in the case of prepare_sql
		// we audit as the test "test select" doesn't go through mysql_execute_command
		if (pThdPrintedList->is_thd_printed_queue[pThdPrintedList->cur_index] == 0 
				|| strncmp(pThdData->getCmdName(),audit_plugin_prepare_sql_str, strlen(audit_plugin_prepare_sql_str)) == 0)
		{
			Audit_handler::log_audit_all(pThdData);
			pThdPrintedList->is_thd_printed_queue[pThdPrintedList->cur_index] = 1;
		}
		else // duplicate no need to audit then simply return
		{
			return;
		}
	}
	else
	{
		Audit_handler::log_audit_all(pThdData);
	}
}

#if MYSQL_VERSION_ID <= 50700
#else
static void query_str_set_thd(THD* thd, const char* sql, size_t lenght) ;
#endif 


#if MYSQL_VERSION_ID <=50700
static int  audit_simple_execute (THD* thd, char *rawbuff, uint length, Parser_state* parser_state, const char* sql, size_t sql_length)
{

  int error __attribute__((unused));
  DBUG_ENTER("audit_simple_execute_rules");
  bool err = false;
  thd->set_query_and_id((char*)sql, sql_length, thd->charset(), next_query_id());

  Parser_state parser_state_new;
  if (parser_state_new.init(thd, thd->query(), sql_length))
  {
      thd->protocol->end_statement();
	  DBUG_RETURN(1);	
  }
	
  lex_start(thd);
  mysql_reset_thd_for_next_command(thd);

  if (query_cache_send_result_to_client(thd, (char*)sql, sql_length) <= 0)
  {
    LEX *lex= thd->lex;

    err= parse_sql(thd, &parser_state_new, NULL);

    const char *found_semicolon= parser_state->m_lip.found_semicolon;
    size_t      qlen= found_semicolon
                      ? (found_semicolon - thd->query())
                      : thd->query_length();
    if (!err)
    {
      if (!(opt_log_raw || thd->slave_thread) || opt_slow_log || opt_bin_log)
      {
        mysql_rewrite_query(thd);

        if (thd->rewritten_query.length())
          lex->safe_to_cache_query= false; 
      }

      if (!(opt_log_raw || thd->slave_thread))
      {
        if (thd->rewritten_query.length())
          general_log_write(thd, COM_QUERY, thd->rewritten_query.c_ptr_safe(),
                                            thd->rewritten_query.length());
        else
          general_log_write(thd, COM_QUERY, thd->query(), qlen);
      }
	}//!err	
    if (!err)
    {
#ifndef NO_EMBEDDED_ACCESS_CHECKS
      if (mqh_used && thd->get_user_connect() &&
      check_mqh(thd, lex->sql_command))
      {
  		  thd->net.error = 0;
      }
      else
#endif
      {
    if (!err)
    {
          if (found_semicolon && (ulong) (found_semicolon - thd->query()))
            thd->set_query_inner(thd->query(),
                                 (uint32) (found_semicolon -
                                           thd->query() - 1),
                                 thd->charset());
          /* Actually execute the query */
          if (found_semicolon)
          {
            lex->safe_to_cache_query= 0;
            thd->server_status|= SERVER_MORE_RESULTS_EXISTS;
          }
            error= mysql_execute_command(thd);
	    }
      }
	}
    else
    {
      DBUG_PRINT("info",("Command aborted. Fatal_error: %d",
             thd->is_fatal_error));

      query_cache_abort(&thd->query_cache_tls);
    }

    THD_STAGE_INFO(thd, stage_freeing_items);
    sp_cache_enforce_limit(thd->sp_proc_cache, stored_program_cache_size);
    sp_cache_enforce_limit(thd->sp_func_cache, stored_program_cache_size);
    thd->end_statement();
    thd->cleanup_after_query();
  }//if query_cache	
  else{
	    if (!opt_log_raw)
    	  general_log_write(thd, COM_QUERY, thd->query(), thd->query_length());
	    parser_state->m_lip.found_semicolon= NULL;
  }

  DBUG_RETURN(err);
}
#else
static int  audit_simple_execute (THD* thd, Parser_state* parser_state, const char* sql, size_t length)
{
  int error MY_ATTRIBUTE((unused));
  DBUG_ENTER("audit_simple_execute_rules");
  DBUG_PRINT("audit_simple_execute_rules", ("query: '%s'", thd->query().str));

  DBUG_EXECUTE_IF("parser_debug", turn_parser_debug_on(););
  
	Parser_state parser_state_new;

  if (parser_state_new.init(thd, sql, length))
  {
      thd->send_statement_status();
	  DBUG_RETURN(1);	
  }

  mysql_reset_thd_for_next_command(thd);

  query_str_set_thd (thd, sql, length);		
  lex_start(thd);

  thd->m_parser_state= &parser_state_new;
  thd->m_parser_state= NULL;

  enable_digest_if_any_plugin_needs_it(thd, &parser_state_new);
  bool err = false;

  if (query_cache.send_result_to_client(thd, thd->query()) <= 0)
  {
    LEX *lex= thd->lex;
    const char *found_semicolon;

    if (!err)
    {
      err= parse_sql(thd, &parser_state_new, NULL);
      if (!err) 
        err= invoke_post_parse_rewrite_plugins(thd, false);
      found_semicolon= parser_state->m_lip.found_semicolon;
    }

    if (!err)
    {
      bool general= !(opt_general_log_raw || thd->slave_thread);

      if (general)
      {
        if (thd->rewritten_query.length())
          query_logger.general_log_write(thd, COM_QUERY,
                                         thd->rewritten_query.c_ptr_safe(),
                                         thd->rewritten_query.length());
        else
        {
          size_t qlen= found_semicolon
            ? (found_semicolon - thd->query().str)
            : thd->query().length;
          
          query_logger.general_log_write(thd, COM_QUERY,
                                         thd->query().str, qlen);
        }
      }
    }

    if (!err)
    {
#ifndef NO_EMBEDDED_ACCESS_CHECKS
      if (mqh_used && thd->get_user_connect() &&
          check_mqh(thd, lex->sql_command))
      {
        if (thd->is_classic_protocol())
          thd->get_protocol_classic()->get_net()->error = 0;
      }
      else
#endif
      {
	if (!err)
	{
          if (found_semicolon && (ulong) (found_semicolon - thd->query().str))
            thd->set_query(thd->query().str,
                           static_cast<size_t>(found_semicolon -
                                               thd->query().str - 1));
          /* Actually execute the query */
          if (found_semicolon)
          {
            lex->safe_to_cache_query= 0;
            thd->server_status|= SERVER_MORE_RESULTS_EXISTS;
          }

          error= mysql_execute_command(thd, true);
		}
      }
    }
    else
    {

      DBUG_PRINT("info",("Command aborted. Fatal_error: %d",
			 thd->is_fatal_error));

      query_cache.abort(&thd->query_cache_tls);
    }

    THD_STAGE_INFO(thd, stage_freeing_items);
    sp_cache_enforce_limit(thd->sp_proc_cache, stored_program_cache_size);
    sp_cache_enforce_limit(thd->sp_func_cache, stored_program_cache_size);
    thd->end_statement();
    thd->cleanup_after_query();
	
  }
  else
  {
    if (!opt_general_log_raw)
      query_logger.general_log_write(thd, COM_QUERY, thd->query().str,
                                     thd->query().length);
    parser_state->m_lip.found_semicolon= NULL;
  }
  #if MYSQL_VERSION_ID <=50700
	  DBUG_RETURN(err || error);
  #else
	  DBUG_RETURN(err);
  #endif 
}
#endif //endof simple_execute def

static bool calc_str_md5 (const char* str, char* digest_str);

#if MYSQL_VERSION_ID <50700
static int  audit_create_rules (THD* thd, char* rawbuff, uint length, Parser_state *parser_state, const char* pattern_name, const char* pattern, const char* actionStr)
#else
static int  audit_create_rules (THD* thd, Parser_state *parser_state, const char* pattern_name, const char* pattern, const char* actionStr)
#endif 
{
	char sql_text_buff[2200]={0};
	char md5Str[128] = {0};
	
	calc_str_md5(pattern, md5Str);
	
	sprintf (sql_text_buff, "insert into mysql.sql_pattern values ('%s','%s','%s','%s')", pattern_name, pattern, md5Str, actionStr) ;
#if MYSQL_VERSION_ID <=50700
	int res = audit_simple_execute (thd, rawbuff, length, parser_state, sql_text_buff, strlen(sql_text_buff)) ;
#else
	int res = audit_simple_execute (thd, parser_state, sql_text_buff, strlen(sql_text_buff)) ;
#endif 	
	if (!res) { //when audit_simple_execute succeed the rule has been inserted, so need only reload the hash table.and the name is pk,so we insert name first
		if (  !insertElemName2PatternCache (pattern_name, pattern, md5Str, actionStr) &&
				 !insertElemPattern2NameCache (pattern, pattern_name)) //insert ok.
			return false ;	
	}
	
	return res; 
}

#if MYSQL_VERSION_ID <= 50700
static int audit_update_rules (THD* thd, char* rawbuff, uint length, Parser_state* parser_state, const char* pattern_name, const char* pattern, const char* actionStr)
#else
static int audit_update_rules (THD* thd, Parser_state* parser_state, const char* pattern_name, const char* pattern, const char* actionStr)
#endif 
{
    char sql_text_buff[1024]={0};
	char md5Str[128] = {0};
	
	calc_str_md5(pattern, md5Str);

    sprintf (sql_text_buff, \
			"update mysql.sql_pattern set sql_pattern = '%s', sql_md5 ='%s', sql_action ='%s' where sql_text='%s' ", \
					pattern, md5Str, actionStr, pattern_name) ;

#if MYSQL_VERSION_ID <= 50700
    int res = audit_simple_execute (thd, rawbuff, length, parser_state, sql_text_buff, strlen(sql_text_buff)) ;
#else
    int res = audit_simple_execute (thd, parser_state, sql_text_buff, strlen(sql_text_buff)) ;
#endif 
    if (!res) { //when audit_simple_execute succeed the rule has been inserted, so need only reload the hash table.
		if (updateElemName2PatternCache (pattern_name, pattern, md5Str, actionStr) || 
					updateElemPattern2NameCache (pattern, pattern_name))
			res = true; 
    }
	return res; 
}

#if MYSQL_VERSION_ID <=50700
static int audit_drop_rules (THD* thd, char* rawbuff, uint length, Parser_state* parser_state, const char* pattern_name)
#else
static int audit_drop_rules (THD* thd, Parser_state* parser_state, const char* pattern_name)
#endif 
{
    char sql_text_buff[1024]={0};
    sprintf (sql_text_buff, \
            "delete from mysql.sql_pattern  where sql_text='%s' ", \
               		 pattern_name) ;

	char* sql_pattern_str = NULL; 
#if MYSQL_VERSION_ID <=50700
    int res = audit_simple_execute (thd, rawbuff, length, parser_state, sql_text_buff, strlen(sql_text_buff)) ;
#else
    int res = audit_simple_execute (thd, parser_state, sql_text_buff, strlen(sql_text_buff)) ;
#endif 

	if (!res) { //when audit_simple_execute succeed the rule has been inserted, so need only reload the hash table.
		Name2Pattern* name2pattern = getPatternByPatternName (pattern_name) ;
		if (!name2pattern) return true; 
		
		sql_pattern_str = strdup (name2pattern->sql_pattern_str) ; //the name2pattern obj perhaps be freed in deleteElemByPatternName,so we dup these.
		if (!deleteElemByPatternName (pattern_name) && 
				!deleteElemByPattern(pattern_name, sql_pattern_str)) {
				if (sql_pattern_str) 
					free (sql_pattern_str);
			return false; 	
		}
    }
	if (sql_pattern_str) 
		free (sql_pattern_str);
    return res; 
}
#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
static int  (*trampoline_send_result_to_client)(Query_cache *pthis, THD *thd, char *sql, uint query_length) = NULL;
#else
static int  (*trampoline_send_result_to_client)(Query_cache *pthis, THD *thd, const LEX_CSTRING& sql_query) = NULL;
#endif

#if 0
#if defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 100108
static bool (*trampoline_open_tables)(THD *thd, const DDL_options_st &options, TABLE_LIST **start, uint *counter, uint flags,
                Prelocking_strategy *prelocking_strategy) = NULL;
#elif MYSQL_VERSION_ID > 50505
static bool (*trampoline_open_tables)(THD *thd, TABLE_LIST **start, uint *counter, uint flags,
                Prelocking_strategy *prelocking_strategy) = NULL;
#else
static int (*trampoline_open_tables)(THD *thd, TABLE_LIST **start, uint *counter, uint flags) = NULL;
#endif
#endif

QueryTableInf *Audit_formatter::getQueryCacheTableList1(THD *thd)
{
	return (QueryTableInf*)	THDVAR(thd, query_cache_table_list);
}
#if 0
static bool (*trampoline_check_table_access)(THD *thd, ulong want_access,TABLE_LIST *tables,
#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID >= 50505
						bool any_combination_of_privileges_will_do,
#endif
						uint number, bool no_errors) = NULL;

#endif
#if 0
static bool audit_check_table_access(THD *thd, ulong want_access,TABLE_LIST *tables,
#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID >= 50505
		bool any_combination_of_privileges_will_do,
#endif
		uint number, bool no_errors) 
{
	TABLE_LIST *pTables;
	bool res = trampoline_check_table_access(thd, want_access, tables,
#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID >= 50505
						any_combination_of_privileges_will_do,
#endif
						number, no_errors);
	if (!res &&  tables)
	{
		pTables = tables;
		QueryTableInf * pQueryTableInf =(QueryTableInf*) THDVAR (thd,query_cache_table_list);
		if (pQueryTableInf)
		{
			while (pTables)
			{
				if (pQueryTableInf->num_of_elem < MAX_NUM_QUERY_TABLE_ELEM && pQueryTableInf->num_of_elem >= 0)
				{
					pQueryTableInf->db[pQueryTableInf->num_of_elem] = (char*) thd_alloc(thd, strlen(Audit_formatter::table_get_db_name(pTables)) + 1);
					strcpy(pQueryTableInf->db[pQueryTableInf->num_of_elem], Audit_formatter::table_get_db_name(pTables));

					pQueryTableInf->table_name[pQueryTableInf->num_of_elem] = (char*) thd_alloc(thd, strlen(Audit_formatter::table_get_name(pTables)) + 1);
					strcpy(pQueryTableInf->table_name[pQueryTableInf->num_of_elem], Audit_formatter::table_get_name(pTables));

					pQueryTableInf->object_type[pQueryTableInf->num_of_elem] = Audit_formatter::retrieve_object_type(pTables);

					pQueryTableInf->num_of_elem++;
				}
				pTables = pTables->next_global;
			}
		}
	}
	return res;
}
#endif 
#if 0
static unsigned int trampoline_check_table_access_size = 0;
#endif

#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
static int  audit_send_result_to_client(Query_cache *pthis, THD *thd, char *sql, uint query_length)
#else
static int  audit_send_result_to_client(Query_cache *pthis, THD *thd, const LEX_CSTRING& sql_query)
#endif
{
	int res;
	void *pList = thd_alloc(thd, sizeof (QueryTableInf));

	if (pList)
	{
		memset(pList,0,sizeof (QueryTableInf));
		THDVAR(thd, query_cache_table_list) =(ulong)pList;
	}

#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
	res = trampoline_send_result_to_client(pthis,thd, sql, query_length);
#else
	res = trampoline_send_result_to_client(pthis, thd, sql_query);
#endif
	if (res)
	{
		if (function_type == FUNC_AUDIT || function_type == FUNC_BOTH) {
        	ThdSesData thdData(thd, ThdSesData::SOURCE_QUERY_CACHE);
	        thdData.storeErrorCode();
    	    audit(&thdData);
		}
	}
	THDVAR(thd, query_cache_table_list) = 0;
	return res;
}

static unsigned int trampoline_send_result_to_client_size = 0;

#if 0
#if defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 100108
static bool audit_open_tables(THD *thd, const DDL_options_st &options, TABLE_LIST **start, uint *counter, uint flags,
                Prelocking_strategy *prelocking_strategy)
{
	bool res;
	res = trampoline_open_tables (thd, options, start, counter, flags, prelocking_strategy);
#elif MYSQL_VERSION_ID > 50505
static bool audit_open_tables(THD *thd, TABLE_LIST **start, uint *counter, uint flags,
                Prelocking_strategy *prelocking_strategy)
{
	bool res;
	res = trampoline_open_tables (thd, start, counter, flags, prelocking_strategy);
#else
static int audit_open_tables(THD *thd, TABLE_LIST **start, uint *counter, uint flags)
{
	int res;
	res = trampoline_open_tables (thd, start, counter, flags);
#endif
	// only log if thread id or query id is non 0 (otherwise this is comming from startup activity)
	if (   (before_after_mode == AUDIT_BEFORE || before_after_mode == AUDIT_BOTH)
	    && (Audit_formatter::thd_inst_thread_id(thd)
			    || Audit_formatter::thd_inst_query_id(thd)))
	{
        if (function_type == FUNC_AUDIT || function_type == FUNC_BOTH) { 
			ThdSesData thd_data (thd);
			audit(&thd_data);
		}
	}
	return res;
}

static unsigned int trampoline_open_tables_size = 0;
#endif

// called by log_slow_statement and general audit event caught by audit interface
static void audit_post_execute(THD * thd)
{
	// only audit non query events
	// query events are audited by mysql execute command
	if (Audit_formatter::thd_inst_command(thd) != COM_QUERY)
	{
		ThdSesData thdData(thd);
		if((strcasestr(thdData.getCmdName(), audit_plugin_show_fields_str) == NULL) 
			&& 	(function_type == FUNC_AUDIT ||  function_type == FUNC_BOTH))
		{
			thdData.storeErrorCode();
			audit(&thdData);
		}
	}
}


/*
 Plugin descriptor
 */
// in 5.6 we use the AUDIT plugin interface. In 5.1/5.5 we just use the general DAEMON plugin

#if MYSQL_VERSION_ID < 50600

static int plugin_type = MYSQL_DAEMON_PLUGIN;
static struct st_mysql_daemon audit_plugin = { MYSQL_DAEMON_INTERFACE_VERSION };

#else

#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
static void audit_notify(THD *thd, unsigned int event_class,
        const void * event)
#else
static int audit_notify(THD *thd, mysql_event_class_t event_class,
        const void * event)
#endif
{
	if (thd == NULL)	// can happen in replication setup
	{
#if ! defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 50709
		return 0;	// return success, keep MySQL going
#else
		return;
#endif
	}

	if (MYSQL_AUDIT_GENERAL_CLASS == event_class)
	{
		const struct mysql_event_general *event_general =
			(const struct mysql_event_general *) event;
		if (MYSQL_AUDIT_GENERAL_STATUS == event_general->event_subclass)
		{
			audit_post_execute(thd);
		}
		else if (MYSQL_AUDIT_GENERAL_ERROR == event_general->event_subclass)
		{
			ThdSesData ThdData(thd);

			// Prior to MySQL 5.6.32 and 5.7.14, we could detect failed
			// logins for existing users with bad passwords in the code
			// for MYSQL_AUDIT_CONNECTION class (in the retrive_command
			// function, called from the ThdSesData constructor).
			//
			// From those versions, we only get access denied indications in
			// this auditing class with MYSQL_AUDIT_GENERAL_ERROR.  Therefore
			// we build the failed login message here for all cases, since
			// we get such an indication for both non-existant users and
			// existing users but with a bad password.
			switch (event_general->general_error_code) {
				case ER_ACCESS_DENIED_ERROR:
				case ER_ACCESS_DENIED_NO_PASSWORD_ERROR:
#ifdef ER_ACCOUNT_HAS_BEEN_LOCKED
				case ER_ACCOUNT_HAS_BEEN_LOCKED:{
#else
				{
#endif
	        		if (function_type == FUNC_AUDIT || function_type == FUNC_BOTH ) { 
						ThdData.setCmdName(audit_plugin_failed_str);
						ThdData.setErrorCode(event_general->general_error_code);
						audit(&ThdData);
					}
				} break;
				default:
					break;
			}
		}
	}
	else if (MYSQL_AUDIT_CONNECTION_CLASS == event_class)
	{
		const struct mysql_event_connection *event_connection =
			(const struct mysql_event_connection *) event;
		// only audit for connect and change_user. disconnect is caught by general event
		if (event_connection->event_subclass != MYSQL_AUDIT_CONNECTION_DISCONNECT
#if !defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 50709
		    // in pre-authenticate, user info etc is empty. don't log it
		    && event_connection->event_subclass != MYSQL_AUDIT_CONNECTION_PRE_AUTHENTICATE
#endif
		)
		{
			if (function_type == FUNC_AUDIT ||
            	function_type == FUNC_BOTH	
	        ) { 
				ThdSesData ThdData(thd);
				audit(&ThdData);
			}
		}
	}
#if ! defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 50709
	return 0;	// Zero means success, MySQL continues processing the event.
#endif
}

static int plugin_type = MYSQL_AUDIT_PLUGIN;
static struct st_mysql_audit audit_plugin =
{
	MYSQL_AUDIT_INTERFACE_VERSION,	/* interface version    */
	NULL,				/* release_thd function */
	audit_notify,			/* notify function      */
#if ! defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 50709
	// As of 5.7.9, this is an array of bitmaps of events that we're interested in.
	{ (unsigned long) MYSQL_AUDIT_GENERAL_ALL,
	  (unsigned long) MYSQL_AUDIT_CONNECTION_ALL,
	  0,
	  0,
	  0,
	  0,
	  0,
	  0,
	  (unsigned long) MYSQL_AUDIT_COMMAND_ALL,
	  0,
	  0
	}
#else
	{ (unsigned long) MYSQL_AUDIT_GENERAL_CLASSMASK |
		MYSQL_AUDIT_CONNECTION_CLASSMASK } /* class mask */
#endif
};

#endif

// some extern definitions which are not in include files
extern void log_slow_statement(THD *thd);

#if MYSQL_VERSION_ID <= 50700
extern int mysql_execute_command(THD *thd);
#else
extern int mysql_execute_command(THD *thd, bool first_level);
#endif 

#if MYSQL_VERSION_ID <=50700
extern void mysql_parse (THD *thd, char *rawbuf, uint length, Parser_state *parser_state);
#else
extern void mysql_parse (THD *thd, Parser_state *parser_state);
#endif 

#if defined(MARIADB_BASE_VERSION)
extern void end_connection(THD *thd);
static int (*trampoline_end_connection)(THD *thd) = NULL;
static unsigned int trampoline_end_connection_size = 0;
#endif

#if MYSQL_VERSION_ID >= 50505
// in 5.5 builtins is named differently
#define mysqld_builtins mysql_mandatory_plugins
#endif
extern struct st_mysql_plugin *mysqld_builtins[];

void remove_mysql_parse_hot_function ()
{
	if (trampoline_mysql_parse_size) {
		void * target_parse_func = NULL; 
	    //remove the mysql_parse trampoline function handler.
#if MYSQL_VERSION_ID <=50700
    	target_parse_func = (void*) (void (*)(THD* thd, char* rawbuff, uint length, Parser_state* parser_state)) &mysql_parse;
#else
    	target_parse_func = (void*) (void (*)(THD* thd, Parser_state* parser_state)) &mysql_parse;
#endif 
	    remove_hot_patch_function(target_parse_func,
    	        (void*) trampoline_mysql_parse,
        	    trampoline_mysql_parse_size, true);
	    trampoline_mysql_parse_size = 0;
	}
}
void remove_hot_functions()
{
	void * target_function = NULL;
	void * target_parse_func = NULL; 

#if MYSQL_VERSION_ID < 50600
	target_function = (void *) log_slow_statement;
	remove_hot_patch_function(target_function,
			(void*) trampoline_log_slow_statement, trampoline_log_slow_statement_size, true);
	trampoline_log_slow_statement_size = 0;
#endif

#if MYSQL_VERSION_ID < 50505
	target_function = (void *) check_user;
	remove_hot_patch_function(target_function,
			(void*) trampoline_check_user, trampoline_check_user_size, true);
	trampoline_check_user_size = 0;
#elif MYSQL_VERSION_ID < 50600
	target_function = (void *) acl_authenticate;
	remove_hot_patch_function(target_function,
			(void*) trampoline_acl_authenticate, trampoline_acl_authenticate_size, true);
	trampoline_acl_authenticate_size = 0;
#endif

#if 0
#if defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 100108
	target_function = (void *)*(bool (*)(THD *thd, const DDL_options_st &options, TABLE_LIST **start, uint *counter, uint flags,
				Prelocking_strategy *prelocking_strategy)) &open_tables;
#elif MYSQL_VERSION_ID > 50505
	target_function = (void *)*(bool (*)(THD *thd, TABLE_LIST **start, uint *counter, uint flags,
				Prelocking_strategy *prelocking_strategy)) &open_tables;
#else
	target_function = (void *)*(int (*)(THD *thd, TABLE_LIST **start, uint *counter, uint flags)) &open_tables;
#endif
	remove_hot_patch_function(target_function,
			(void*) trampoline_open_tables, trampoline_open_tables_size, true);
	trampoline_open_tables_size = 0;
#endif 

#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
	int (Query_cache::*pf_send_result_to_client)(THD *,char *, uint) = &Query_cache::send_result_to_client;
#else
	int (Query_cache::*pf_send_result_to_client)(THD *,const LEX_CSTRING&) = &Query_cache::send_result_to_client;
#endif
	target_function = *(void **) &pf_send_result_to_client;
	remove_hot_patch_function(target_function,
			(void*) trampoline_send_result_to_client, trampoline_send_result_to_client_size, true);
	trampoline_send_result_to_client_size = 0;

#if 0
	remove_hot_patch_function((void*) check_table_access,
			(void*) trampoline_check_table_access,
			trampoline_check_table_access_size, true);
	trampoline_check_table_access_size=0;
#endif 

#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
	target_function = (void*) (int (*)(THD *thd)) &mysql_execute_command;
#else
	target_function = (void*)
		(int (*)(THD *thd, bool first_level)) &mysql_execute_command;
#endif

#if defined(MARIADB_BASE_VERSION)
	target_function = (void*) end_connection;
	remove_hot_patch_function(target_function,
			(void*) trampoline_end_connection,
			trampoline_end_connection_size, true);
	trampoline_end_connection_size = 0;
#endif

	remove_hot_patch_function(target_function,
			(void*) trampoline_mysql_execute_command,
			trampoline_mysql_execute_size, true);
	trampoline_mysql_execute_size = 0;

	//remove the mysql_parse trampoline function handler.	
#if MYSQL_VERSION_ID <=50700
	target_parse_func = (void*) (void (*)(THD* thd, Parser_state* parser_state)) &mysql_parse;
#else
	target_parse_func = (void*) (void (*)(THD* thd, char* rawbuff, uint length, Parser_state* parser_state)) &mysql_parse;
#endif 
	remove_hot_patch_function(target_parse_func,
			(void*) trampoline_mysql_parse,
			trampoline_mysql_parse_size, true);
	trampoline_mysql_parse_size = 0;
}

int is_remove_patches(ThdSesData *pThdData)
{
	static bool called_once = false;
	const char *cmd = pThdData->getCmdName();
	const char *sUninstallPlugin = "uninstall_plugin";
	LEX *pLex = Audit_formatter::thd_lex(pThdData->getTHD());
	if (pThdData->getTHD() && pLex != NULL && strncasecmp(cmd, sUninstallPlugin, strlen(sUninstallPlugin)) == 0)
	{
#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
		LEX_STRING Lex_comment = *(LEX_STRING*)(((unsigned char *) pLex) + Audit_formatter::thd_offsets.lex_comment);
#else
		Sql_cmd_uninstall_plugin *up = Audit_formatter::lex_sql_cmd(pLex);
		LEX_STRING Lex_comment = *(LEX_STRING*)(((unsigned char *) up) + Audit_formatter::thd_offsets.uninstall_cmd_comment);
#endif
		if (strncasecmp(Lex_comment.str, "AUDIT", 5) == 0)
		{
			char msgBuffer[200]={0};
			if (! uninstall_plugin_enable)
			{	
				sprintf(msgBuffer, "Uninstall %s plugin disabled", "AUDIT");
				my_message(ER_NOT_ALLOWED_COMMAND, msgBuffer, MYF(0));
				return 2;
			}

			Audit_handler::stop_all();
			remove_hot_functions();

			if (! called_once)
			{
                called_once = true;
                sprintf(msgBuffer, "Uninstall AUDIT plugin must be called again to complete");
                my_message(WARN_PLUGIN_BUSY, msgBuffer, MYF(0));
				return 2;
			}
			return 1;
		}
	}
	return 0;
}
#if MYSQL_VERSION_ID <= 50700
extern "C" LEX_STRING* thd_query_string (MYSQL_THD thd) ;
#else
extern "C" LEX_CSTRING thd_query_unsafe(MYSQL_THD thd);
#endif 

static const char *query_str_from_thd(THD *thd, size_t *len)
{
#if MYSQL_VERSION_ID <= 50700
	LEX_STRING *pstr = NULL; 
	pstr = thd_query_string(thd);
	if (pstr->length) {
		*len =  pstr->length; 
		return pstr->str; 
	} 	
#else
	const LEX_CSTRING str = thd_query_unsafe(thd);
	if (str.length > 0)
	{
		*len = str.length;
		return str.str;
	}
#endif 
	*len = 0;
	return NULL;
}

#if MYSQL_VERSION_ID <=50700
#else
static void query_str_set_thd (THD* thd, const char* query_str, size_t query_length) 
{
	DBUG_ASSERT(current_thd == thd) ;
	thd->set_query(const_cast<char*> (query_str), query_length);
}
#endif 

Diagnostics_area* thd_da_unsafte (MYSQL_THD thd) 
{
	DBUG_ASSERT(current_thd == thd);
	Diagnostics_area* da (NULL) ; 
	da = thd-> get_stmt_da() ;

	return da; 
}

#if MYSQL_VERSION_ID <= 50700
static void doRulesAction (THD *thd, char* rawbuff, uint length, Parser_state *parser_state, ASTNode* rules)
#else
static void doRulesAction (THD *thd, Parser_state *parser_state, ASTNode* rules)
#endif 
{
	DBUG_ENTER("doRulesAction");
	DBUG_ASSERT(rules->getNodeType() == ASTNode::NODE_TYPE_CREATERULE_STMT ||
                    rules->getNodeType() == ASTNode::NODE_TYPE_UPDATERULE_STMT ||
                    rules->getNodeType() == ASTNode::NODE_TYPE_DROPRULE_STMT
				);
	
	switch (rules->getNodeType()) {
		case ASTNode::NODE_TYPE_CREATERULE_STMT : {
			 CreateRuleStmt* create = (CreateRuleStmt*) rules;
             const char* pattern_name(NULL), *pattern(NULL), *actionStr(NULL); 
             pattern_name = (create->sqlText());
             pattern = (create->sqlPattern());
             actionStr = (create->sqlAction());
#if MYSQL_VERSION_ID <=50700
	         audit_create_rules (thd,rawbuff, length, parser_state, pattern_name, pattern, actionStr);
#else
	         audit_create_rules (thd,parser_state, pattern_name, pattern, actionStr);
#endif 
		} break; 
		case ASTNode::NODE_TYPE_UPDATERULE_STMT : {
             UpdateRuleStmt* update = (UpdateRuleStmt*) rules;
             const char* pattern_name(NULL), *pattern(NULL), *actionStr(NULL);
             pattern_name = update->sqlText();
             pattern = update->sqlPattern();
             actionStr = update->sqlAction();
#if MYSQL_VERSION_ID <=50700
             audit_update_rules(thd, rawbuff, length, parser_state, pattern_name, pattern, actionStr) ;
#else
             audit_update_rules(thd, parser_state, pattern_name, pattern, actionStr) ;
#endif 
		}break;
		case ASTNode::NODE_TYPE_DROPRULE_STMT : {
             DropRuleStmt* drop = (DropRuleStmt*) rules;
             const char* pattern_name(NULL);
             pattern_name = drop->sqlText();
#if MYSQL_VERSION_ID <= 50700
		     audit_drop_rules (thd, rawbuff, length, parser_state, pattern_name) ;	
#else					
		     audit_drop_rules (thd, parser_state, pattern_name) ;	
#endif 
		} break; 
		default:
			break;
	};
	DBUG_VOID_RETURN ;
}
static void doSqlAction (THD *thd, Parser_state *parser_state, SelectStmt* select)
{
	DBUG_ENTER("doSqlAction");
	DBUG_ASSERT(select->getNodeType() != ASTNode::NODE_TYPE_CREATERULE_STMT &&
                    select->getNodeType() != ASTNode::NODE_TYPE_UPDATERULE_STMT &&
                    select->getNodeType() != ASTNode::NODE_TYPE_DROPRULE_STMT
				);
	const char* pattText = (select)->toString(); 
	Pattern2Name* pattern2name = NULL ;
	const char* actionStr = NULL; 
	pattern2name = getPatterNameByPattern ( pattText );	

    if (pattern2name) {
       Name2Pattern* name2pattern  = getPatternByPatternName (pattern2name->pattern_name);
       if (!name2pattern)
  			DBUG_VOID_RETURN;

       actionStr = name2pattern->sql_action;
       if (!strncasecmp(actionStr,"ignore", strlen("ignore"))) {
  			DBUG_VOID_RETURN;
       }else if (!strncasecmp(actionStr,"killsession", strlen("killsession"))) {
#if MYSQL_VERSION_ID <= 50700
          close_connection(thd);
#else
          close_connection(thd, 0, false, false);
#endif 
       } else if (!strncasecmp(actionStr,"user-defined", strlen("user-defined"))) {
			
	   }
   }//if pattern2name	

  DBUG_VOID_RETURN;
}

#if MYSQL_VERSION_ID <= 50700
static void audit_mysql_parse(THD *thd, char *rawbuff, uint length, Parser_state *parser_state)
#else
static void audit_mysql_parse(THD *thd, Parser_state *parser_state) 
#endif 
{
	DBUG_ENTER("audit_mysql_parse");
	if (!isMapInitialized()) {
    	if (sql_pattern_param_init (false)) {
			Diagnostics_area **stmt_da = ((Diagnostics_area **) (((unsigned char *) thd)
					                        + Audit_formatter::thd_offsets.stmt_da));
			
			(*stmt_da)->set_error_status (Diagnostics_area::DA_ERROR, "   unable to init cache." ,"Error"); 			
            sql_print_error(get_text("unable to init pattern2name or name2pattern cache. Aborting."));
			Diagnostics_area **stmt_da = ((Diagnostics_area **) (((unsigned char *) thd)
					                        + Audit_formatter::thd_offsets.stmt_da));
			
			Diagnostics_area* da_ptr = *stmt_da; 
			#if MYSQL_VERSION_ID <= 50700
				da_ptr->set_error_status (Diagnostics_area::DA_ERROR, "   unable to init cache." ,"Error", NULL); 
			#else
				da_ptr->set_error_status (Diagnostics_area::DA_ERROR, "   unable to init cache." ,"Error"); 
			#endif 
            DBUG_VOID_RETURN;
        }
   
        if (load_sql_pattern (thd)){
			Diagnostics_area **stmt_da = ((Diagnostics_area **) (((unsigned char *) thd)
					                        + Audit_formatter::thd_offsets.stmt_da));
			
			(*stmt_da)->set_error_status (Diagnostics_area::DA_ERROR, "   unable to load data from sql_pattern into cache." ,"Error"); 			
            sql_print_error(get_text("unable to load data into pattern2name or name2pattern cache. Aborting."));
			Diagnostics_area **stmt_da = ((Diagnostics_area **) (((unsigned char *) thd)
					                        + Audit_formatter::thd_offsets.stmt_da));
			
			Diagnostics_area* da_ptr = *stmt_da; 
			#if MYSQL_VERSION_ID <=50700
				da_ptr->set_error_status (Diagnostics_area::DA_ERROR, "   unable to load data from sql_pattern into cache." ,"Error", NULL); 
			#else
				da_ptr->set_error_status (Diagnostics_area::DA_ERROR, "   unable to load data from sql_pattern into cache." ,"Error"); 
			#endif 
            DBUG_VOID_RETURN;
        }
    }
    
	ThdSesData thd_data(thd);
	do_delay(& thd_data);	
#if  defined(MARIADB_BASE_VERSION)
	if (Audit_formatter::thd_killed(thd) >= KILL_CONNECTION)
#else
	if (Audit_formatter::thd_killed(thd) == THD::KILL_CONNECTION)
#endif
	{
	}
	else
	{
		switch (is_remove_patches(&thd_data))
		{
		case 1:
			// hot patch function were removed and we call the real execute (restored)
#if MYSQL_VERsION_ID <= 50700
			mysql_execute_command(thd);
#else
			mysql_execute_command(thd, true);
#endif 
			break;
		case 2:
			// denied uninstall  plugin
			break;
		default: {
			// everything else
			if (function_type == FUNC_AUDIT) { //not firewall or both mode, just return
#if MYSQL_VERSION_ID <=50700

				trampoline_mysql_parse(thd, rawbuff, length, parser_state);
#else
				trampoline_mysql_parse(thd, parser_state);
#endif 
				DBUG_VOID_RETURN;
			}

			size_t qlen =0; 	
			const char *query = query_str_from_thd(thd_data.getTHD(), &qlen);
			if (strcasestr (query, "show") || strcasestr(query,"shutdown")) {
#if MYSQL_VERSION_ID <= 50700
				trampoline_mysql_parse(thd, rawbuff, length, parser_state);
#else
				trampoline_mysql_parse(thd, parser_state);
#endif
				DBUG_VOID_RETURN;
			} 
			Parser my_parser;
		    ASTNode* sql = my_parser.raw_parser(query);
		    if (sql) {
				if (sql->getNodeType() == ASTNode::NODE_TYPE_CREATERULE_STMT ||
					sql->getNodeType() == ASTNode::NODE_TYPE_UPDATERULE_STMT ||
					sql->getNodeType() == ASTNode::NODE_TYPE_DROPRULE_STMT
				) {
#if MYSQL_VERSION_ID <= 50700
					doRulesAction (thd, rawbuff, length, parser_state, sql) ;
#else
					doRulesAction (thd, parser_state, sql) ;
#endif 
					sql->release();
					delete sql;
					DBUG_VOID_RETURN ;
				} else {
					SelectStmt* select = (SelectStmt*) sql; 
					doSqlAction (thd, parser_state, select);
				}
				sql->release();
				delete sql;
			} else {
				goto do_return_imm ;
       	    }//fi
do_return_imm:
#if MYSQL_VERSION_ID <=50700
		   trampoline_mysql_parse(thd, rawbuff, length, parser_state);
#else
		   trampoline_mysql_parse(thd, parser_state);
#endif 
		  }//default
		}//swith
	}//else
	DBUG_VOID_RETURN;
}

/*
 * Override functions for hot patch + audit. We call our audit function
 * after the execute command so all tables are resolved.
 */
static int audit_mysql_execute_command(THD *thd)
{
	bool firstTime = false;
	THDPRINTED *pThdPrintedList = GetThdPrintedList(thd);

	if (pThdPrintedList)
	{
		if (pThdPrintedList->cur_index < (MAX_NUM_QUEUE_ELEM - 1))
		{
			pThdPrintedList->cur_index++;
			pThdPrintedList->is_thd_printed_queue[pThdPrintedList->cur_index] = 0;
		}
	}
	else
	{
		firstTime = true;
		pThdPrintedList = (THDPRINTED *) thd_alloc (thd, sizeof(THDPRINTED));
		if (pThdPrintedList)
		{
			memset(pThdPrintedList, 0, sizeof(THDPRINTED));
			// pThdPrintedList->cur_index = 0;
			THDVAR(thd,is_thd_printed_list) = (ulong) pThdPrintedList;
		}
	}

	ThdSesData thdData(thd);
	const char *cmd = thdData.getCmdName();

	do_delay(&thdData);
	
	if (before_after_mode == AUDIT_BEFORE || before_after_mode == AUDIT_BOTH)
	{
		if ((strcasestr(cmd, "alter") != NULL ||
		    strcasestr(cmd, "drop") != NULL ||
		    strcasestr(cmd, "create") != NULL ||
		    strcasestr(cmd, "truncate") != NULL ||
		    strcasestr(cmd, "rename") != NULL) && 
			(function_type == FUNC_AUDIT || function_type == FUNC_BOTH))
		{
			audit(&thdData);
		}
	}

	int res;
#if  defined(MARIADB_BASE_VERSION)
	if (Audit_formatter::thd_killed(thd) >= KILL_CONNECTION)
#else
	if (Audit_formatter::thd_killed(thd) == THD::KILL_CONNECTION)
#endif
	{
		res = 1;
	}
	else
	{
		switch (is_remove_patches(&thdData))
		{
		case 1:
			// hot patch function were removed and we call the real execute (restored)
#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
			res = mysql_execute_command(thd);
#else
			res = mysql_execute_command(thd, false);
#endif
			break;
		case 2:
			// denied uninstall  plugin
			res = 1;
			break;
		default:
			// everything else
#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
			res = trampoline_mysql_execute_command(thd);
#else
			res = trampoline_mysql_execute_command(thd, false);
#endif 
		}
	}


	if ((before_after_mode == AUDIT_AFTER || before_after_mode == AUDIT_BOTH) &&
			(function_type == FUNC_AUDIT || function_type == FUNC_BOTH))
	{
		audit(&thdData);
	}

	if (pThdPrintedList && pThdPrintedList->cur_index > 0)
	{
		pThdPrintedList->cur_index--;
	}

	if (firstTime)
	{
		THDVAR(thd,is_thd_printed_list) = 0;
	}
	return res;

}

#if MYSQL_VERSION_ID < 50600
static void audit_log_slow_statement(THD *thd)
{
	trampoline_log_slow_statement(thd);
	audit_post_execute(thd);
}
#endif

#if MYSQL_VERSION_ID < 50505
static int audit_check_user(THD *thd, enum enum_server_command command,
	       const char *passwd, uint passwd_len, const char *db,
	       bool check_count)
{
	int res = trampoline_check_user(thd, command, passwd, passwd_len, db, check_count);
    if (function_type == FUNC_AUDIT ||
         function_type == FUNC_BOTH )
    { 
			ThdSesData thdData(thd);
			thdData.storeErrorCode();
			audit(&thdData);
	}
	return (res);
}
#elif MYSQL_VERSION_ID < 50600
// only for 5.5
// in 5.6: we use audit plugin event to get the login event
static bool audit_acl_authenticate(THD *thd, uint connect_errors, uint com_change_user_pkt_len)
{
	bool res = trampoline_acl_authenticate(thd, connect_errors, com_change_user_pkt_len);
    if (function_type == FUNC_AUDIT || function_type == FUNC_BOTH) { 
		ThdSesData thdData(thd);
		thdData.storeErrorCode();
		audit(&thdData);
	}
	return (res);
}
#endif

#if defined(MARIADB_BASE_VERSION)
static void audit_end_connection(THD *thd)
{
#if MYSQL_VERSION_ID < 50600
    if (function_type == FUNC_AUDIT || function_type == FUNC_BOTH) { 
		ThdSesData thd_data(thd);
		thd_data.setCmdName("Quit");
		audit(&thd_data);
	}
#endif
	PeerInfo *peer = (PeerInfo *) THDVAR(thd, peer_info);
	if (peer)
	{
		free(peer);
		THDVAR(thd, peer_info) = 0;
	}
	trampoline_end_connection(thd);
}
#endif

static bool parse_thd_offsets_string (char *poffsets_string)
{
	char offset_str[2048] = {0};
	char *poffset_str = offset_str;

	strncpy(poffset_str,poffsets_string,array_elements(offset_str));

	char *comma_delimiter = strchr(poffset_str, ',');
	size_t i = 0;
	OFFSET *pOffset;
	size_t len = strlen(poffset_str);

	for (size_t j = 0; j < len; j++)
	{
		if (! (isdigit(poffset_str[j]) || poffset_str[j] == ' ' || poffset_str[j] == ','))
		{
			return false;
		}
	}
	while (poffset_str != NULL)
	{
		comma_delimiter = strchr(poffset_str, ',');
		if (comma_delimiter)
		{
			*comma_delimiter = '\0';
		}

		pOffset = (OFFSET *) &Audit_formatter::thd_offsets.query_id + i;
		if ((size_t)pOffset- (size_t)&Audit_formatter::thd_offsets < sizeof(Audit_formatter::thd_offsets))
		{
			if (sscanf(poffset_str, "%zu", pOffset) != 1)
			{
				sql_print_error("%s Failed parsing audit_offsets: scanf failed for offset string: \"%s\" (possible missing offset)", log_prefix, poffset_str);
				return false;
			}
		}
		else
		{
			sql_print_error("%s Failed parsing audit_offsets: too many offsets specified", log_prefix);
			return false;
		}

		i++;

		if (comma_delimiter)
		{
			poffset_str = comma_delimiter + 1;
		}
		else
		{
			poffset_str = NULL;
		}
	}

	// validate that we got all offsets. If there is still space in thd_offsets then we didn't get all offsets
	pOffset = (OFFSET*)&Audit_formatter::thd_offsets.query_id + i;
	if ((size_t)pOffset- (size_t)&Audit_formatter::thd_offsets < sizeof (Audit_formatter::thd_offsets))
	{
		sql_print_error("%s Failed parsing audit_offsets: not all offsets specified. This may happen if you used an old version of offset-extract.sh script.", log_prefix);
		return false;
	}
	return true;
}

static bool validate_offsets(const ThdOffsets *offset)
{
#if !defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 50709
	if (offset->lex_m_sql_command == 0 || offset->uninstall_cmd_comment == 0)
	{
		sql_print_error("%s Offsets: missing offsets for checking 'uninstall plugin'", log_prefix);
		return false;
	}
#endif
	// check that offsets are actually correct. We use a buff of memory as a dummy THD (32K is high enough)
	char buf[32*1024] = {0};
	THD *thd = (THD *)buf;
	
	// sanity check that offsets match

	// we set the thread id to a value using the offset and then check that the value matches what thd_get_thread_id returns
	const my_thread_id thread_id_test_val = 123456;
	(*(my_thread_id *) (((char *) thd)+ offset->thread_id)) = thread_id_test_val;
	my_thread_id res = thd_get_thread_id(thd);
	if (res != thread_id_test_val)
	{
		sql_print_error(
				"%s Offsets: %s (%s) match thread validation check fails with value: %lu. Skipping offset.",
				log_prefix, offset->version, offset->md5digest,
				(unsigned long) res);
		return false;
	}

	// extended validation via security_context method
	// can be disabled via: audit_validate_offsets_extended=OFF
	//Here we do not use this option. we do not want to confuse our DABs, the less options, the more concise.
	
	validate_offsets_extended_enable = true; 
	if (validate_offsets_extended_enable)
	{
		const query_id_t query_id_test_val = 789;
		(*(query_id_t *) (((char *) thd)+ offset->query_id)) = query_id_test_val;

		Security_context *sctx = (Security_context *) (((unsigned char *) thd) + offset->main_security_ctx);
		char user_test_val[] = "aud_tusr";

		if (! offset->sec_ctx_user) // use compiled header
		{
#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
			sctx->user = user_test_val;
#else
			sctx->set_user_ptr(user_test_val, strlen(user_test_val));
#endif
		}
		else
		{
			(*(const char **) (((unsigned char *) sctx) + offset->sec_ctx_user)) = user_test_val;
		}
/*
		char buffer[2048] = {0};
		thd_security_context(thd, buffer, 2048, 1000);

		// verfiy our buffer contains query id
		if (strstr(buffer, " 789") == NULL || strstr(buffer, user_test_val) == NULL)
		{
			sql_print_error(
					"%s Offsets: %s (%s) sec context validation check fails with value: %s. Skipping offset.",
					log_prefix, offset->version, offset->md5digest, buffer);
			return false;
		}
		sql_print_information(
				"%s extended offsets validate res: %s", log_prefix, buffer);
*/
	}
	return true;
}

/**
 * Calculate md5 sum of a file.
 *
 * @file_name: file to calc md5 for
 * @digest_str: string to fill with digest result should be big enought to hold 32 chars
 *
 * @return true on success.
 */
static bool calc_str_md5 (const char* str, char* digest_str)
{
	unsigned char digest[16] = {0};

	my_MD5Context context;
	my_MD5Init(&context);

	my_MD5Update(&context, (void*)str, strlen(str));

	my_MD5Final(digest, &context);

	for (int j = 0; j < 16; j++)
	{
		sprintf(&(digest_str[j * 2]), "%02x", digest[j]);
	}
	return true;	
}
static bool calc_file_md5(const char *file_name, char *digest_str)
{
	File fd;
	unsigned char digest[16] = {0};
	bool ret = false;
	
	char pidFilename[100];
	sprintf(pidFilename, "/proc/%d/exe", getpid());
	if ((fd = my_open(pidFilename, O_RDONLY, MYF(MY_WME))) < 0)
	{
        sql_print_error("%s Failed file open: [%s], errno: %d. Retrying with /proc/%d/exe.",
                log_prefix, file_name, errno, getpid());

        char pidFilename[100];
        sprintf(pidFilename, "/proc/%d/exe", getpid());
        if ((fd = my_open(pidFilename, O_RDONLY, MYF(MY_WME))) < 0)
        {
            sql_print_error("%s Failed file open: [%s], errno: %d.",
                log_prefix, pidFilename, errno);
            return false;
        }
	}

	my_MD5Context context;
	my_MD5Init(&context);
	const size_t buff_size = 16384;
	unsigned char file_buff[buff_size] = {0};

	ssize_t res;
	do
	{
		res = read(fd, file_buff, buff_size);
		if (res > 0)
		{
			my_MD5Update(&context, file_buff, res);
		}
	}
	while (res > 0);

	if (res == 0) // reached end of file
	{
		my_MD5Final(digest, &context);
		ret = true;
	}
	else
	{
		sql_print_error("%s Failed program read. res: %zd, errno: %d.",
				log_prefix, res, errno);
	}

	(void) my_close(fd, MYF(0));

	if (ret) // we got the digest
	{
		for (int j = 0; j < 16; j++)
		{
			sprintf(&(digest_str[j * 2]), "%02x", digest[j]);
		}
	}
	return ret;
}

/**
 * Setup the offsets needs to extract data from THD.
 *
 * return 0 on success otherwise 1
 */
static int setup_offsets()
{
	DBUG_ENTER("setup_offsets");
	sql_print_information ("%s setup_offsets audit_offsets: %s validate_checksum: %d offsets_by_version: %d",
			log_prefix, offsets_string, validate_checksum_enable, offsets_by_version_enable);

	char digest_str[128] = {0};
	const ThdOffsets *offset;

	// setup digest_str to contain the md5sum in hex
	calc_file_md5(my_progname, digest_str);

	sql_print_information(
			"%s mysqld: %s (%s) ", log_prefix, my_progname, digest_str);

	// if present in my.cnf
	// [mysqld]
	// audit_validate_checksum=1
	// or if
	// audit_checksum=0f4d7e3b17eb36f17aafe4360993a769
	// if (validate_checksum_enable || (checksum_string != NULL && strlen(checksum_string) > 0))
	// {

	// if present the offset_string specified in my.cnf
	// [mysqld]
	// audit_offsets=6200, 6264, 3672, 3944, 88, 2048

	if (offsets_string != NULL)
	{
		if (checksum_string != NULL && strlen(checksum_string) > 0)
		{
			if (strncasecmp(checksum_string, digest_str, 32))
			{
				sql_print_information(
						"%s checksum check failed for %s, but found %s",
						log_prefix, checksum_string, digest_str);
				DBUG_RETURN(1);
			}
		}

		if (parse_thd_offsets_string(offsets_string))
		{
			sql_print_information ("%s setup_offsets Audit_formatter::thd_offsets values: %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu", log_prefix,
					Audit_formatter::thd_offsets.query_id,
					Audit_formatter::thd_offsets.thread_id,
					Audit_formatter::thd_offsets.main_security_ctx,
					Audit_formatter::thd_offsets.command,
					Audit_formatter::thd_offsets.lex,
					Audit_formatter::thd_offsets.lex_comment,
					Audit_formatter::thd_offsets.sec_ctx_user,
					Audit_formatter::thd_offsets.sec_ctx_host,
					Audit_formatter::thd_offsets.sec_ctx_ip,
					Audit_formatter::thd_offsets.sec_ctx_priv_user,
					Audit_formatter::thd_offsets.db,
					Audit_formatter::thd_offsets.killed,
					Audit_formatter::thd_offsets.client_capabilities,
					Audit_formatter::thd_offsets.pfs_connect_attrs,
					Audit_formatter::thd_offsets.pfs_connect_attrs_length,
					Audit_formatter::thd_offsets.pfs_connect_attrs_cs,
					Audit_formatter::thd_offsets.net,
					Audit_formatter::thd_offsets.lex_m_sql_command,
					Audit_formatter::thd_offsets.uninstall_cmd_comment,
					Audit_formatter::thd_offsets.found_rows,
					Audit_formatter::thd_offsets.sent_row_count,
					Audit_formatter::thd_offsets.row_count_func,
                    Audit_formatter::thd_offsets.stmt_da,
                    Audit_formatter::thd_offsets.da_status,
                    Audit_formatter::thd_offsets.da_sql_errno
			);

			if (! validate_offsets(&Audit_formatter::thd_offsets))
			{
				sql_print_error("%s Offsets set didn't pass validation. audit_offsets: %s .", log_prefix, offsets_string);
				DBUG_RETURN(1);
			}
		}
		else
		{
			sql_print_error("%s Failed parsing audit_offsets: %s", log_prefix, offsets_string);
			DBUG_RETURN(1);
		}

		sql_print_information ("%s Validation passed. Using offsets from audit_offsets: %s",log_prefix, offsets_string);
		DBUG_RETURN(0);
		// exit from function
	}

	size_t arr_size =  thd_offsets_arr_size;
	// iterate and search for the first offset which matches our checksum
	if (validate_checksum_enable && strlen(digest_str) > 0)
	{
		for (size_t i=0; i < arr_size; i++)
		{
			offset = thd_offsets_arr + i;
			if (strlen(offset->md5digest) > 0)
			{
				if (strncasecmp(digest_str, offset->md5digest, 32) == 0)
				{
					sql_print_information("%s Checksum verified. Using offsets from offset version: %s (%s)", log_prefix, offset->version, digest_str);
					Audit_formatter::thd_offsets = *offset;
					DBUG_RETURN(0);
					// return
				}
			}
		}
	}

	if (offsets_by_version_enable)
	{
		bool server_is_ndb = strstr(server_version, "ndb") != NULL;
		for (size_t i = 0; i < arr_size; i++)
		{
			offset = thd_offsets_arr + i;
			const char *version = offset->version;
			bool version_is_ndb = strstr(offset->version, "ndb") != NULL;
			const char * dash = strchr(version, '-');
			char version_stripped[16] = {0};
			if (dash) // we use the version string up to the '-'
			{
				size_t tocopy = dash - version;
				if (tocopy > 15)
					tocopy = 15; // sanity
				strncpy(version_stripped, version, tocopy);
				version = version_stripped;
			}

			if (strstr(server_version, version))
			{
				if (server_is_ndb == version_is_ndb)
				{
					if (validate_offsets(offset))
					{
						sql_print_information("%s Using offsets from offset version: %s (%s)", log_prefix, offset->version, offset->md5digest);
						Audit_formatter::thd_offsets = *offset;
						DBUG_RETURN(0);
					}
					else
					{
						// try doing 24 byte decrement on THD offsets.
						// Seen that on Ubuntu/Debian this is valid. On 5.6 this is 16 bytes.
#if MYSQL_VERSION_ID < 50600
						OFFSET dec = 24;
#else
						OFFSET dec = 16;
#endif
						ThdOffsets decoffsets = *offset;
						decoffsets.query_id -= dec;
						decoffsets.thread_id -= dec;
						decoffsets.main_security_ctx -= dec;
						decoffsets.command -= dec;
						if(decoffsets.killed)
						{
							decoffsets.killed -= dec;            
						}
						if(decoffsets.client_capabilities)
						{
							decoffsets.client_capabilities -= dec;  
						}
						if(decoffsets.net)
						{
							decoffsets.net -= dec;  
						}
						if (decoffsets.lex_m_sql_command)
						{
							decoffsets.lex_m_sql_command -= dec;
						}
						if (decoffsets.uninstall_cmd_comment)
						{
							decoffsets.uninstall_cmd_comment -= dec;
						}
						if (decoffsets.found_rows)
						{
							decoffsets.found_rows -= dec;
						}
						if (decoffsets.sent_row_count)
						{
							decoffsets.sent_row_count -= dec;
						}
						if (decoffsets.row_count_func)
						{
							decoffsets.row_count_func -= dec;
						}
                        if (decoffsets.stmt_da)
                        {
                            decoffsets.stmt_da -= dec;
                        }
                        if (decoffsets.da_status)
                        {
                            decoffsets.da_status -= dec;
                        }
                        if (decoffsets.da_sql_errno)
                        {
                            decoffsets.da_sql_errno -= dec;
                        }

						if (validate_offsets(&decoffsets))
						{
							Audit_formatter::thd_offsets = decoffsets;
							sql_print_information("%s Using decrement (%zu) offsets from offset version: %s (%s) values: %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu",
									log_prefix, dec, offset->version, offset->md5digest,
									Audit_formatter::thd_offsets.query_id,
									Audit_formatter::thd_offsets.thread_id,
									Audit_formatter::thd_offsets.main_security_ctx,
									Audit_formatter::thd_offsets.command,
									Audit_formatter::thd_offsets.lex,
									Audit_formatter::thd_offsets.lex_comment,
									Audit_formatter::thd_offsets.sec_ctx_user,
									Audit_formatter::thd_offsets.sec_ctx_host,
									Audit_formatter::thd_offsets.sec_ctx_ip,
									Audit_formatter::thd_offsets.sec_ctx_priv_user,
									Audit_formatter::thd_offsets.killed,
									Audit_formatter::thd_offsets.client_capabilities,
									Audit_formatter::thd_offsets.pfs_connect_attrs,
									Audit_formatter::thd_offsets.pfs_connect_attrs_length,
									Audit_formatter::thd_offsets.pfs_connect_attrs_cs,
									Audit_formatter::thd_offsets.net,
									Audit_formatter::thd_offsets.lex_m_sql_command,
									Audit_formatter::thd_offsets.uninstall_cmd_comment,
									Audit_formatter::thd_offsets.found_rows,
									Audit_formatter::thd_offsets.sent_row_count,
									Audit_formatter::thd_offsets.row_count_func,
                                    Audit_formatter::thd_offsets.stmt_da,
                                    Audit_formatter::thd_offsets.da_status,
                                    Audit_formatter::thd_offsets.da_sql_errno
							);

							DBUG_RETURN(0);
						}
					}
				} // ndb check
#if defined(__x86_64__) && MYSQL_VERSION_ID > 50505
				else if (server_is_ndb)
				{
					// in 64bit 5.5 we've seen ndb has an offset of 32 on first 2 values
					OFFSET inc = 32;
					ThdOffsets incoffsets = *offset;
					incoffsets.query_id += inc;
					incoffsets.thread_id += inc;
					if (validate_offsets(&incoffsets))
					{
						Audit_formatter::thd_offsets = incoffsets;
						sql_print_information("%s Using increment (%zu) offsets from offset version: %s (%s) values: %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu",
								log_prefix, inc, offset->version, offset->md5digest,
								Audit_formatter::thd_offsets.query_id,
								Audit_formatter::thd_offsets.thread_id,
								Audit_formatter::thd_offsets.main_security_ctx,
								Audit_formatter::thd_offsets.command,
								Audit_formatter::thd_offsets.lex,
								Audit_formatter::thd_offsets.lex_comment,
								Audit_formatter::thd_offsets.sec_ctx_user,
								Audit_formatter::thd_offsets.sec_ctx_host,
								Audit_formatter::thd_offsets.sec_ctx_ip,
								Audit_formatter::thd_offsets.sec_ctx_priv_user,
								Audit_formatter::thd_offsets.killed,
								Audit_formatter::thd_offsets.client_capabilities,
								Audit_formatter::thd_offsets.pfs_connect_attrs,
								Audit_formatter::thd_offsets.pfs_connect_attrs_length,
								Audit_formatter::thd_offsets.pfs_connect_attrs_cs,
								Audit_formatter::thd_offsets.net,
								Audit_formatter::thd_offsets.lex_m_sql_command,
								Audit_formatter::thd_offsets.uninstall_cmd_comment,
								Audit_formatter::thd_offsets.found_rows,
								Audit_formatter::thd_offsets.sent_row_count,
								Audit_formatter::thd_offsets.row_count_func,
                                Audit_formatter::thd_offsets.stmt_da,
                                Audit_formatter::thd_offsets.da_status,
                                Audit_formatter::thd_offsets.da_sql_errno

						);
						DBUG_RETURN(0);
					}
				}
#endif
			}
		}
	}

	sql_print_information("%s Couldn't find proper THD offsets for: %s", log_prefix, server_version);
	DBUG_RETURN(1);
}


const char *retrieve_command(THD *thd, bool &is_sql_cmd)
{
	const char *cmd = NULL;
	is_sql_cmd = false;
	int command = Audit_formatter::thd_inst_command(thd);
	if (command < 0 || command > COM_END)
	{
		command = COM_END;
	}

	// check if from query cache. If so set to select and return
	if (THDVAR(thd, query_cache_table_list) != 0)
	{
		return "select";
	}

	const int sql_command = thd_sql_command(thd);

#if defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 100108
#define MAX_SQL_COM_INDEX SQLCOM_END
#else
#define MAX_SQL_COM_INDEX MAX_COM_STATUS_VARS_RECORDS
#endif

	if (command != COM_STMT_PREPARE && sql_command >= 0 && sql_command < MAX_SQL_COM_INDEX)
	{
		is_sql_cmd = true;
		cmd = com_status_vars_array[sql_command].name;
	}

	if (! cmd)
	{
		cmd = command_name[command].str;
	}

#if MYSQL_VERSION_ID < 50600
	const char *user = Audit_formatter::thd_inst_main_security_ctx_user(thd);
	const char *priv_user = Audit_formatter::thd_inst_main_security_ctx_priv_user(thd);
	if (strcmp(cmd, audit_plugin_conn_str) == 0 &&
	    ((user && strncmp(user, audit_plugin_event_sch_str, strlen (audit_plugin_event_sch_str)) != 0) &&
	    (priv_user == NULL || *priv_user == 0x0)))
	{
		cmd = audit_plugin_failed_str;
	}
#endif
	return cmd;
}

static int set_com_status_vars_array()
{
	DBUG_ENTER("set_com_status_vars_array");
	SHOW_VAR *com_status_vars;
	int sv_idx = 0;

	while (strncmp(status_vars[sv_idx].name,audit_plugin_com_str, strlen(audit_plugin_com_str)) != 0 && status_vars[sv_idx].name != NullS)
	{
		sv_idx++;
	}

	if (strncmp(status_vars[sv_idx].name, audit_plugin_com_str, strlen(audit_plugin_com_str)) == 0)
	{
		com_status_vars = (SHOW_VAR *) status_vars[sv_idx].value;
		int status_vars_index = 0;
		// we use "select" as 0 offset (SQLCOM_SELECT=0)

		while (strncmp(com_status_vars[status_vars_index].name,audit_plugin_select_str, strlen(audit_plugin_select_str)) != 0 
				&& com_status_vars[status_vars_index].name != NullS)
		{
			status_vars_index++;
		}

		if (strncmp(com_status_vars[status_vars_index].name,audit_plugin_select_str, strlen(audit_plugin_select_str)) != 0)
		{
			sql_print_error(get_text("%s Failed finding 'select' index in com_status_vars: [%p]. Plugin Init failed."),
					log_prefix, com_status_vars);
			DBUG_RETURN (1);
		}

		size_t initial_offset = (size_t) com_status_vars[status_vars_index].value;
		status_vars_index = 0;

		while (com_status_vars[status_vars_index].name != NullS)
		{
			int sql_command_idx = ((size_t)(com_status_vars[status_vars_index].value) -  (initial_offset)) / sizeof (ulong);
			if (sql_command_idx >= 0 && sql_command_idx < MAX_COM_STATUS_VARS_RECORDS)
			{
				com_status_vars_array [sql_command_idx].name = com_status_vars[status_vars_index].name;
				com_status_vars_array [sql_command_idx].type = com_status_vars[status_vars_index].type;
				com_status_vars_array [sql_command_idx].value = com_status_vars[status_vars_index].value;
			}
			status_vars_index++;
		}
		sql_print_information(get_text("%s Done initializing sql command names. status_vars_index: [%d], com_status_vars: [%p]."),
				log_prefix, status_vars_index, com_status_vars);
	}
	else
	{
		sql_print_error(get_text("%s Failed looking up 'Com' entry in status_vars. Plugin Init failed."),
				log_prefix);
		DBUG_RETURN (1);
	}
	DBUG_RETURN (0);
}

static int string_to_array(const void *save, void *array, int rows, int length)
{
	const char *save_string;
	save_string = *static_cast<const char* const *> (save);
	char *string_array;
	string_array = (char *) array;
	int r = 0;

	if (save_string != NULL)
	{
		int p = 0;
		int last_char = 0; // points to the index after last non-whitespace char (used to trim whitespace at end)
		for (int i = 0; save_string[i] != '\0'; i++)
		{
			if (0 == p && isspace(save_string[i]))
			{
				// leading white space. trim it out
				continue;
			}
			if (save_string[i] == ',')
			{
				if (p > 0)
				{
					string_array[r * length + last_char] = '\0';
					last_char = 0;
					p = 0;
					r++;
					if (r == (rows - 1))
					{
						break;
					}
				}
			}
			else
			{
				string_array[r * length + p] = tolower(save_string[i]);
				p++;
				if (!isspace(save_string[i]))
				{
					last_char = p;
				}
			}
		}
		if (last_char > 0)
		{
			string_array[r * length + last_char] = '\0';
			r++;
		}
		string_array[r * length + 0] = '\0';
	}
	return r;
}

__attribute__ ((noinline)) static void trampoline_dummy_func_for_mem()
{
	TRAMPOLINE_NOP_DEF
}

// holds memory used for trampoline
static void *trampoline_mem = NULL;

// pointer to current free mem
static void *trampoline_mem_free = NULL;

/**
 * Utility method for hot patching
 */
static int do_hot_patch(void ** trampoline_func_pp, unsigned int * trampoline_size,
	void* target_function, void* audit_function,  const char * func_name)
{
	// 16 byte align the pointer
	DATATYPE_ADDRESS addrs = (DATATYPE_ADDRESS)trampoline_mem_free + 15;
	*trampoline_func_pp = (void*)(addrs & ~0x0F);

	// hot patch functions
	unsigned int used_size;
	int res = hot_patch_function(target_function, audit_function,
			*trampoline_func_pp, trampoline_size, &used_size, true);
	if (res != 0)
	{
		// hot patch failed.
		sql_print_error(get_text("%s unable to hot patch %s (%p). res: %d."),
				log_prefix, func_name, target_function, res);
		return 1;
	}
	sql_print_information(
			get_text("%s hot patch for: %s (%p) complete. Audit func: %p, Trampoline address: %p, size: %u, used: %u."),
			log_prefix, func_name, target_function, audit_function, *trampoline_func_pp, *trampoline_size, used_size);
	trampoline_mem_free = (void *)(((DATATYPE_ADDRESS)*trampoline_func_pp) + used_size);
	return 0;
}

#define DECLARE_STRING_ARR_UPDATE_FUNC(NAME) \
static void NAME ## _string_update(THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save)\
{\
	num_ ## NAME = string_to_array(save, NAME ## _array, array_elements( NAME ## _array), sizeof( NAME ## _array[0]));\
	/* handle "set global audit_xxx = null;" */ \
	char *const* save_p = static_cast<char*const*>(save);\
	if (save_p != NULL && *save_p != NULL)\
	{\
		strncpy( NAME ##_buff , *static_cast<char*const*>(save), array_elements( NAME ## _buff) - 1);\
	}\
	else\
	{\
		NAME ## _buff[0] = '\0';\
	}\
	NAME ## _string = NAME ##_buff;\
	sql_print_information("%s Set " #NAME " num: %d, value: %s", log_prefix, num_ ## NAME, NAME ## _string);\
}

DECLARE_STRING_ARR_UPDATE_FUNC(delay_cmds)
DECLARE_STRING_ARR_UPDATE_FUNC(whitelist_cmds)
DECLARE_STRING_ARR_UPDATE_FUNC(record_cmds)
DECLARE_STRING_ARR_UPDATE_FUNC(password_masking_cmds)
DECLARE_STRING_ARR_UPDATE_FUNC(whitelist_users)
DECLARE_STRING_ARR_UPDATE_FUNC(record_objs)

static void password_masking_regex_string_update(THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save)
{
	const char *str_val = "";
	char *const* save_p = static_cast<char*const*>(save);

	// if got a null pointer or empty string, use ""
	if (save_p != NULL && *save_p != NULL)
	{
		str_val = *save_p;
	}

	// if a string value supplied, check that it compiles
	if (*str_val)
	{
		bool res = json_formatter.compile_password_masking_regex(str_val);
		if (! res)	// fails compilation
		{
			// copy in default pw
			strncpy(password_masking_regex_buff, default_pw_masking_regex, array_elements(password_masking_regex_buff) - 1);
			password_masking_regex_string = password_masking_regex_buff;
			// compile it
			json_formatter.compile_password_masking_regex(password_masking_regex_string);
		}
		else
		{
			// all ok, save it
			if (str_val != password_masking_regex_buff)
			{
				strncpy(password_masking_regex_buff, str_val, array_elements(password_masking_regex_buff) - 1);
			}
			password_masking_regex_string = password_masking_regex_buff;
			// it was already compiled, don't need to do it again
		}
		sql_print_information(get_text("%s Compile password_masking_regex  res: [%d]"), log_prefix, res);
	}
	else
	{
		// clear out password
		password_masking_regex_buff[0] = '\0';
		password_masking_regex_string = password_masking_regex_buff;
		json_formatter.compile_password_masking_regex(password_masking_regex_string);
	}

	sql_print_information(get_text("%s Set password_masking_regex  value: [%s]"), log_prefix, str_val);
}

static void replace_char(char *str, const char tofind, const char rplc)
{
	size_t n = strlen(str);
	for (size_t i = 0; i< n; i++)
	{
		if (tofind == str[i])
		{
			str[i] = rplc;
		}
	}
}

static void json_socket_name_update(THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save)
{
	const char *str_val = NULL;
	char *const* save_p = static_cast<char*const*>(save);

	if (save_p != NULL && *save_p != NULL)
	{
		str_val = *save_p;
	}

	const char *str = str_val;
	const size_t buff_len = array_elements(json_socket_name_buff) - 1;

	// copy str to buffer only if str is not pointing to buff
	if (NULL == str)
	{
		json_socket_name_buff[0] = '\0';
	}
	else if (str != json_socket_name_buff)
	{
		strncpy(json_socket_name_buff, str, buff_len);
	}

	if (strlen(json_socket_name_buff) == 0) // set default
	{
		const char *name_prefix = "/var/run/db-audit/mysql.audit_";

		size_t indx = strlen(name_prefix); // count how much to move forward the buff
		strncpy(json_socket_name_buff, name_prefix, buff_len);

		char cwd_buff[512] = {0};
		my_getwd(cwd_buff, array_elements(cwd_buff) - 1, 0);
		replace_char(cwd_buff, '/', '_');
		size_t cwd_len = strlen(cwd_buff);

		if (cwd_len > 0 && '_' != cwd_buff[cwd_len-1]) // add _ to end
		{
			strncpy(cwd_buff + cwd_len, "_", array_elements(cwd_buff) - 1 - cwd_len);
		}
		strncpy(json_socket_name_buff + indx, cwd_buff, buff_len  - indx);
		indx += cwd_len;

		if (indx < buff_len)
		{
			if (! opt_disable_networking)
			{
				unsigned int port = (mysqld_port > 0 ? mysqld_port : MYSQL_DEFAULT_PORT);
				snprintf(json_socket_name_buff + indx, buff_len - indx, "%u", port);
			}
			else if (mysqld_unix_port != NULL && mysqld_unix_port[0] != '\0')
			{
				strncpy(json_socket_name_buff + indx,  mysqld_unix_port, buff_len  - indx);
				replace_char(json_socket_name_buff + indx, '/', '_');
			}
			else
			{
				sql_print_error(get_text("%s MySQL configured without networking and without UNIX-domain socket!"),
					log_prefix);
			}
		}
		else // should never happen
		{
			sql_print_error(get_text("%s json_socket_name_buff not big enough to set default name. buff: %s"),
		                log_prefix, json_socket_name_buff);
		}
	}
	json_socket_handler.m_io_dest = json_socket_name_buff;
	sql_print_information(get_text("%s Set json_socket_name str: [%s] value: [%s]"), log_prefix, str, json_socket_handler.m_io_dest);
}

// check that the regex compiles. Return 0 on success.
static int password_masking_regex_check(THD *thd, struct st_mysql_sys_var *var, void *save, st_mysql_value *value)
{
	int length = array_elements(password_masking_regex_check_buff);
	const char *str = value->val_str(value, password_masking_regex_check_buff, &length);
	*(const char**)save = str;
	if (NULL == str || str[0] == '\0') // empty string is fine (means disabled)
	{
		return 0;
	}
	int res = 1;
	pcre *preg = Audit_json_formatter::regex_compile(str);
	if (preg)
	{
		res = 0;
	}
	pcre_free(preg);
	return res;
}

// extended method to set also record_empty_objs_set
static void record_objs_string_update_extended(THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save)
{
	record_objs_string_update(thd, var, tgt, save);
	if (num_record_objs > 0) // check if to record also the empty set of objects
	{
		const char *objects[] = {"{}", NULL};
		record_empty_objs_set = check_array(objects, (const char *) record_objs_array[0], sizeof(record_objs_array[0]));
	}
	else
	{
			record_empty_objs_set = true;
	}
	sql_print_information(get_text("%s Set record_empty_objs: %d"), log_prefix, record_empty_objs_set);
}
static void max_seg_log_file_size_update (THD* thd, struct st_mysql_sys_var* var __attribute__((unused)), void *target __attribute__((unused)), const void* save)
{
	void* ptr = const_cast<void*> (save) ;
	unsigned long str_value_value  = *(static_cast<unsigned long*>(ptr)) ;
	if (str_value_value <=0)
		str_value_value = _2GB_; 
	max_seg_log_file_size = str_value_value; 
	json_file_handler.set_seg_file_size(max_seg_log_file_size) ;
	
	return ;
}

void install_mysql_parse_hot_function ()
{
	void* target_parse_func = NULL; 
	if (trampoline_mysql_parse_size ==0){
		//the mysql_parse function.

#if MYSQL_VERSION_ID <= 50700
		target_parse_func = (void*)
			(void (*)(THD *thd, char* rawbuf, uint length, Parser_state *parser_state)) &mysql_parse;
#else
		target_parse_func = (void*)
			(void (*)(THD *thd, Parser_state *parser_state)) &mysql_parse;
#endif 		
    	if (do_hot_patch((void **)&trampoline_mysql_parse, &trampoline_mysql_parse_size,
                target_parse_func, (void *)audit_mysql_parse,  "mysql_parse"))
    	{
       		//return (1);
   	 	}
	}
    //return (0);
}
/*
 * Initialize the plugin installation.
 *
 * SYNOPSIS
 * audit_plugin_init()
 *
 * RETURN VALUE
 * 0                    success
 * 1                    failure
 */
static int audit_plugin_init(void *p)
{

	DBUG_ENTER("audit_plugin_init");

#ifdef __x86_64__
	const char * arch = "64bit";
#else
	const char * arch = "32bit";
#endif

	int interface_ver = audit_plugin.interface_version ;
#if MYSQL_VERSION_ID < 50600
	interface_ver = interface_ver >> 8;
#endif
	sql_print_information(
			get_text("%s starting up. Version: %s , Revision: %s (%s). AUDIT plugin interface version: %d (0x%x). MySQL Server version: %s."),
			log_prefix, MYSQL_AUDIT_PLUGIN_VERSION,
			MYSQL_AUDIT_PLUGIN_REVISION, arch, interface_ver, interface_ver,
			server_version);

	// setup our offsets.
#if 0	
	if (!m_parser) {
		m_parser = new Parser ();
	}
#endif 
	if (setup_offsets() != 0)
	{
		DBUG_RETURN(1);
	}
	if(!Audit_formatter::thd_offsets.client_capabilities)
	{
		sql_print_error(
				get_text("%s offsets for client_capabilities not defined. client_capabilities will not be logged."),
				log_prefix);
	}
#ifdef HAVE_SESS_CONNECT_ATTRS
	if(!Audit_formatter::thd_offsets.pfs_connect_attrs ||
			!Audit_formatter::thd_offsets.pfs_connect_attrs_length ||
			!Audit_formatter::thd_offsets.pfs_connect_attrs_cs)
	{
		sql_print_error(
				get_text("%s offsets for sess_connect_attrs not defined. sess_connect_attrs will not be logged."),
				log_prefix);
	}
#endif
	if (!Audit_formatter::thd_offsets.net)
	{
		sql_print_error(get_text("%s offset for NET structure not defined. peer attributes will not be logged."),
				log_prefix);
	}

	if (delay_cmds_string != NULL)
	{
		delay_cmds_string_update(NULL, NULL, NULL, &delay_cmds_string);
	}
	if (whitelist_cmds_string != NULL)
	{
		whitelist_cmds_string_update(NULL, NULL, NULL, &whitelist_cmds_string);
	}
	if (record_cmds_string != NULL)
	{
		record_cmds_string_update(NULL, NULL, NULL, &record_cmds_string);
	}
	if (whitelist_users_string != NULL)
	{
		whitelist_users_string_update(NULL, NULL, NULL, &whitelist_users_string);
	}
	if (record_objs_string != NULL)
	{
		record_objs_string_update_extended(NULL, NULL, NULL, &record_objs_string);
	}
	if (password_masking_cmds_string != NULL)
	{
		password_masking_cmds_string_update(NULL, NULL, NULL, &password_masking_cmds_string);
	}
	if (password_masking_regex_string != NULL)
	{
		password_masking_regex_string_update(NULL, NULL, NULL, &password_masking_regex_string);
	}
	
	// update to generate the default if needed
	json_socket_name_update(NULL, NULL, NULL, &(json_socket_handler.m_io_dest));

	// set the password masking callback for json formatters
	json_formatter.m_perform_password_masking = check_do_password_masking;

	// setup audit handlers (initially disabled)
	int res = json_file_handler.init(&json_formatter);
	if (res != 0)
	{
		sql_print_error(
				get_text("%s unable to init json file handler. res: %d. Aborting."),
				log_prefix, res);
		DBUG_RETURN(1);
	}

	res = json_socket_handler.init(&json_formatter);
	if (res != 0)
	{
		sql_print_error(
				get_text("%s unable to init json socket handler. res: %d. Aborting."),
				log_prefix, res);
		DBUG_RETURN(1);
	}

	// enable according to what we have in *file_handler_enable
	// (this is set accordingly by sysvar functionality)
	json_file_handler.set_enable(json_file_handler_enable);
	json_socket_handler.set_enable(json_socket_handler_enable);
	
	Audit_handler::m_audit_handler_list[Audit_handler::JSON_FILE_HANDLER] = &json_file_handler;
	Audit_handler::m_audit_handler_list[Audit_handler::JSON_SOCKET_HANDLER] = &json_socket_handler;

	// align our trampoline mem on its own page
	const unsigned long page_size = GETPAGESIZE();
	const unsigned long std_page_size = 4096;
	bool use_static_memory = (page_size <= std_page_size);	
	int mmap_flags = MAP_PRIVATE|MAP_ANONYMOUS;
	trampoline_mem = NULL;

#ifdef __x86_64__
	size_t func_in_mysqld = (size_t)log_slow_statement;
	size_t func_in_plugin = (size_t)trampoline_dummy_func_for_mem;
	if (func_in_mysqld < INT_MAX && func_in_plugin > INT_MAX)
	{
		// See comment about IndirectJump in hot_patch.cc.
		//mmap_flags |= MAP_32BIT;		
		trampoline_mem = mmap(NULL, page_size, PROT_READ|PROT_EXEC, mmap_flags | MAP_32BIT, -1, 0);
		if (MAP_FAILED == trampoline_mem)
		{
			trampoline_mem = NULL;
			sql_print_information(get_text("%s unable to mmap 32bit memory size: %lu, errno: %d. This may happen when 32bit address space is used up. Will try static and regular mmap..."),
					log_prefix, page_size, errno);				
		}
		else
		{
			sql_print_information(
					get_text("%s mem via 32bit mmap: %p page size: %ld"), log_prefix, trampoline_mem, page_size);
		}
	}
#endif	
	if (!trampoline_mem)
	{
		if (use_static_memory)
		{
			// use static executable memory we alocated via trampoline_dummy_func_for_mem
			DATATYPE_ADDRESS addrs = (DATATYPE_ADDRESS)trampoline_dummy_func_for_mem + (page_size - 1);
			trampoline_mem = (void*)(addrs & ~(page_size - 1));
			sql_print_information(
					get_text("%s mem func addr: %p mem start addr: %p page size: %ld"),
					log_prefix, trampoline_dummy_func_for_mem, trampoline_mem, page_size);
		}
		else // big pages allocate mem using mmap
		{
			trampoline_mem = mmap(NULL, page_size, PROT_READ|PROT_EXEC, mmap_flags , -1, 0);
			if (MAP_FAILED == trampoline_mem)
			{
				trampoline_mem = NULL;
				sql_print_error(get_text("%s unable to mmap 32bit memory size: %lu, errno: %d. Abborting"),
						log_prefix, page_size, errno);				
				DBUG_RETURN(1);
			}
			else
			{
				sql_print_information(
						get_text("%s mem via mmap: %p page size: %ld"), log_prefix, trampoline_mem, page_size);
			}
		}
	}
	trampoline_mem_free = trampoline_mem;

	// hot patch stuff
	void * target_function = NULL;
		
#if defined(MARIADB_BASE_VERSION) ||  MYSQL_VERSION_ID < 50709
	target_function = (void*) mysql_execute_command;
#else
	target_function = (void*)
		(int (*)(THD *thd, bool first_level)) &mysql_execute_command;
#endif

	if (do_hot_patch((void **)&trampoline_mysql_execute_command, &trampoline_mysql_execute_size,
				target_function, (void *)audit_mysql_execute_command,  "mysql_execute_command"))
	{
		DBUG_RETURN(1);
	}

#if MYSQL_VERSION_ID < 50600
	if (do_hot_patch((void **)&trampoline_log_slow_statement, &trampoline_log_slow_statement_size,
				(void *)log_slow_statement, (void *)audit_log_slow_statement,  "log_slow_statement"))
	{
		sql_print_error(get_text("%s Failed hot patch. Continuing as non-critical."),
				log_prefix);

	}
#endif

#if MYSQL_VERSION_ID < 50505
	if (do_hot_patch((void **)&trampoline_check_user, &trampoline_check_user_size,
				(void *)check_user, (void *)audit_check_user,  "check_user"))
	{
		DBUG_RETURN(1);
	}
#elif MYSQL_VERSION_ID < 50600
	if (do_hot_patch((void **)&trampoline_acl_authenticate, &trampoline_acl_authenticate_size,
				(void *)acl_authenticate, (void *)audit_acl_authenticate,  "acl_authenticate"))
	{
		DBUG_RETURN(1);
	}
#endif


#if defined(MARIADB_BASE_VERSION) || MYSQL_VERSION_ID < 50709
	int (Query_cache::*pf_send_result_to_client)(THD *,char *, uint) = &Query_cache::send_result_to_client;
#else
	int (Query_cache::*pf_send_result_to_client)(THD *,const LEX_CSTRING&) = &Query_cache::send_result_to_client;
#endif
	target_function = *(void **)  &pf_send_result_to_client;
	if (do_hot_patch((void **)&trampoline_send_result_to_client, &trampoline_send_result_to_client_size,
				(void *)target_function, (void *)audit_send_result_to_client,  "send_result_to_client"))
	{
		DBUG_RETURN(1);
	}

//we do not log the open_table and check_table_access oper for performance .
#if 0
	if (do_hot_patch((void **)&trampoline_check_table_access, &trampoline_check_table_access_size,
				(void *)check_table_access, (void *)audit_check_table_access,  "check_table_access"))
	{
		DBUG_RETURN(1);
	}

#if defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 100108
	target_function = (void *)*(bool (*)(THD *thd, const DDL_options_st &options, TABLE_LIST **start, uint *counter, uint flags,
				Prelocking_strategy *prelocking_strategy)) &open_tables;
#elif MYSQL_VERSION_ID > 50505
	target_function = (void *)*(bool (*)(THD *thd, TABLE_LIST **start, uint *counter, uint flags,
				Prelocking_strategy *prelocking_strategy)) &open_tables;
#else
	target_function = (void *)*(int (*)(THD *thd, TABLE_LIST **start, uint *counter, uint flags)) &open_tables;
#endif
	if (do_hot_patch((void **)&trampoline_open_tables, &trampoline_open_tables_size,
				(void *)target_function, (void *)audit_open_tables,  "open_tables"))
	{
		DBUG_RETURN(1);
	}
#endif 

#if defined(MARIADB_BASE_VERSION)
	target_function = (void*) end_connection;
	if (do_hot_patch((void **)&trampoline_end_connection, &trampoline_end_connection_size,
				(void *)target_function, (void *)audit_end_connection,  "end_connection"))
	{
		DBUG_RETURN(1);
	}
#endif

	if (set_com_status_vars_array () != 0)
	{
		DBUG_RETURN(1);
	}

	sql_print_information(get_text("%s Init completed successfully."), log_prefix);
	DBUG_RETURN(0);
}

/*
 * plugin deinstallation.
 *
 * SYNOPSIS
 * audit_plugin_deinit()
 * Does nothing.
 *
 * RETURN VALUE
 * 0                    success
 * 1                    failure (cannot happen)
 */

static int audit_plugin_deinit(void *p)
{
	DBUG_ENTER("audit_plugin_deinit");
	sql_print_information(get_text("%s deinit"), log_prefix);
	remove_hot_functions();
	sql_pattern_param_deinit (true) ;
#if 0
	if (m_parser) {
		delete m_parser ;
		m_parser = NULL; 
	}
#endif	
	DBUG_RETURN(0);
}

/*
 Plugin status variables for SHOW STATUS
 */

static struct st_mysql_show_var audit_status[] =
{
{ "Audit_version",
        (char *) MYSQL_AUDIT_PLUGIN_VERSION "-" MYSQL_AUDIT_PLUGIN_REVISION,
        SHOW_CHAR
#if ! defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 50709
	, SHOW_SCOPE_GLOBAL
#endif
	},
{ "Audit_protocol_version",
		(char *) AUDIT_PROTOCOL_VERSION,
		SHOW_CHAR
#if ! defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID >= 50709
	, SHOW_SCOPE_GLOBAL
#endif
	},
// {"called",     (char *)&number_of_calls, SHOW_LONG},
        { 0, 0, (enum_mysql_show_type) 0 } };



static void json_log_file_enable(THD *thd, struct st_mysql_sys_var *var,
		void *tgt, const void *save)
{
	json_file_handler_enable = *(my_bool *) save ? TRUE : FALSE;
	if (json_file_handler.is_init())
	{
		json_file_handler.set_enable(json_file_handler_enable);
	}
}

static void json_log_file_flush(THD *thd, struct st_mysql_sys_var *var,
		void *tgt, const void *save)
{
	// always set to false. as we just flush if set to true and leave at 0
	json_file_handler_flush = FALSE;
	my_bool val = *(my_bool *) save ? TRUE : FALSE;
	if (val && json_file_handler.is_init())
	{
		json_file_handler.flush();
	}
}

static void json_log_socket_enable(THD *thd, struct st_mysql_sys_var *var,
		void *tgt, const void *save)
{
	json_socket_handler_enable = *(my_bool *) save ? TRUE : FALSE;
	if (json_socket_handler.is_init())
	{
		json_socket_handler.set_enable(json_socket_handler_enable);
	}
}
static void parsing_sql_enable_func (THD *thd, struct st_mysql_sys_var *var,
		void *tgt, const void *save)
{
	parsing_sql_enable = *(my_bool *) save ? TRUE : FALSE;
	json_formatter.m_do_parsing_sql = (parsing_sql_enable) ? TRUE : FALSE ;  
	
	if (parsing_sql_enable) {
		if (function_type == FUNC_FIREWALL || function_type == FUNC_BOTH)
			install_mysql_parse_hot_function(); 

	} else //no do parsing sql statement.
		remove_mysql_parse_hot_function() ; 
		
}

static void dns_svr_addr_update (THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save)
{
    const char *str_val = NULL;
    char *const* save_p = static_cast<char*const*>(save);

    if (save_p != NULL && *save_p != NULL){
        str_val = *save_p;
    }
   
    memset(dns_svr_addr_buffer, '\0', sizeof (dns_svr_addr_buffer));
    if (str_val) {//valid str. use the new one, otherwise, using default val.
        strncpy (dns_svr_addr_buffer, str_val, strlen(str_val)) ;
    }else
        strncpy (dns_svr_addr_buffer, "127.0.0.1", strlen("127.0.0.1"));
   
    json_formatter.m_dns_svr_addr= dns_svr_addr_buffer; //to set the new val.
    return ;
}

#if defined (USIG_IP_AND_PORT)
static void remote_ip_update (THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save)
{
	const char *str_val = NULL;
	char *const* save_p = static_cast<char*const*>(save);

	if (save_p != NULL && *save_p != NULL){
		str_val = *save_p;
	}
	
	memset(remote_ip_buffer, '\0', sizeof (remote_ip_buffer));
    if (str_val) {//valid str. use the new one, otherwise, using default val.
		strncpy (remote_ip_buffer, str_val, strlen(str_val)) ;	
	}else 
		strncpy (remote_ip_buffer, "127.0.0.1", strlen("127.0.0.1"));
	
	json_formatter.m_remote_ip= remote_ip_buffer; //to set the new val.
	return ; 
}
#endif 

static void remote_port_update(THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save)
{
	void* ptr = const_cast<void*> (save) ;
	assert(ptr);
	int new_remote_port  = *(static_cast<int*>(ptr)) ;
	if (new_remote_port <=0)
		new_remote_port = 5000; 

	remote_port = new_remote_port;
	json_formatter.m_remote_port = remote_port; 
	
	return ;
}
static void socket_type_update( THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save )
{
    const char *str_val = NULL;
    char *const* save_p = static_cast<char*const*>(save);

    if (save_p != NULL && *save_p != NULL){
        str_val = *save_p;
    }

	memset(socket_type_buffer,'\0', sizeof(socket_type_buffer));
    if (str_val) {//valid str. use the new one, otherwise, using default val.
	    strncpy (socket_type_buffer, str_val, strlen(str_val)) ;
    }else 
        strncpy (socket_type_buffer, "unix", strlen("unix"));
   
    json_formatter.m_socket_type= socket_type_buffer; //to set the new val.
	return; 
}
static void function_type_update( THD *thd, struct st_mysql_sys_var *var, void *tgt, const void *save )
{
    const char *str_val = NULL;
    char *const* save_p = static_cast<char*const*>(save);

    if (save_p != NULL && *save_p != NULL){
        str_val = *save_p;
    }

    memset(function_type_buffer,'\0', sizeof(function_type_buffer));
    if (str_val) {//valid str. use the new one, otherwise, using default val.
        strncpy (function_type_buffer, str_val, strlen(str_val)) ;
    }else
        strncpy (function_type_buffer, "audit", strlen("audit"));
	function_type_string = function_type_buffer;

	//do type checking, then set the proper target function.		
	if (strncasecmp (function_type_buffer, "audit", strlen("audit")) == 0 ) {
		function_type = FUNC_AUDIT ;
		
		//(json_formatter.m_do_parsing_sql == TRUE)? : install_mysql_parse_hot_function() ;
		//		remove_mysql_parse_hot_function (); 
	}else if (strncasecmp (function_type_buffer, "firewall", strlen("firewall")) == 0){
		function_type  = FUNC_FIREWALL; 	
		(json_formatter.m_do_parsing_sql == TRUE)? 	install_mysql_parse_hot_function () :
				remove_mysql_parse_hot_function(); 
	}
	else if (strncasecmp (function_type_buffer, "both", strlen("both")) == 0) {
		function_type = FUNC_BOTH; 
		(json_formatter.m_do_parsing_sql == TRUE)? 	install_mysql_parse_hot_function () :
				remove_mysql_parse_hot_function(); 
	}else {
		function_type = FUNC_AUDIT;
		remove_mysql_parse_hot_function (); 
	}
    return;
}
// setup sysvars which update directly the relevant plugins

static MYSQL_SYSVAR_BOOL(socket_creds, json_formatter.m_write_socket_creds,
             PLUGIN_VAR_RQCMDARG,
        "AUDIT log socket credentials from Unix Domain Socket. Enable|Disable. Default enabled.", NULL, NULL, 1);

static MYSQL_SYSVAR_BOOL(client_capabilities, json_formatter.m_write_client_capabilities,
             PLUGIN_VAR_RQCMDARG,
        "AUDIT log client capabilities. Enable|Disable. Default disabled.", NULL, NULL, 0);
  
#ifdef HAVE_SESS_CONNECT_ATTRS
static MYSQL_SYSVAR_BOOL(sess_connect_attrs, json_formatter.m_write_sess_connect_attrs,
             PLUGIN_VAR_RQCMDARG,
        "AUDIT log session connect attributes (see: performance_schema.session_connect_attrs table). Enable|Disable. Default enabled.", NULL, NULL, 1);
#endif
		
static MYSQL_SYSVAR_BOOL(header_msg, json_formatter.m_write_start_msg,
             PLUGIN_VAR_RQCMDARG,
        "AUDIT write header message at start of logging or file flush Enable|Disable. Default enabled.", NULL, NULL, 1);

static MYSQL_SYSVAR_BOOL(force_record_logins, force_record_logins_enable,
             PLUGIN_VAR_RQCMDARG,
        "AUDIT force record Connect, Quit and Failed Login commands, regardless of the settings in audit_record_cmds and audit_record_objs  Enable|Disable. Default disabled.", NULL, NULL, 0);

static MYSQL_SYSVAR_STR(json_log_file, json_file_handler.m_io_dest,
        PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_MEMALLOC,
        "AUDIT plugin json log file name",
        NULL, NULL, "audit.log");

static MYSQL_SYSVAR_LONG(json_file_bufsize, json_file_handler.m_bufsize,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json log file buffer size. Buffer size in bytes (larger size may improve performance). 0 = use default size, 1 = no buffering. If changed during runtime need to perform a flush for the new value to take affect.",
        NULL, NULL, 0, 10485760, 10485760, 0);//10Mb

static MYSQL_SYSVAR_UINT(json_file_sync, json_file_handler.m_sync_period,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json log file sync period. If the value of this variable is greater than 0, audit log will sync to disk after every audit_json_file_sync writes.",
        NULL, NULL, 0, 0, UINT_MAX32, 0);

static MYSQL_SYSVAR_UINT(json_file_retry, json_file_handler.m_retry_interval,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json log file retry interval. If the plugin fails to open/write to the json log file, will retry to open every specified interval in seconds. Set for 0 to disable retrying. Default 60 seconds.",
        NULL, NULL, 60, 0, UINT_MAX32, 0);

static MYSQL_SYSVAR_UINT(json_socket_retry, json_socket_handler.m_retry_interval,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json socket connect interval. If the plugin fails to connect/write to the json audit socket, will retry to connect every specified interval in seconds. Set for 0 to disable retrying. Default 10 seconds.",
        NULL, NULL, 10, 0, UINT_MAX32, 0);

static MYSQL_SYSVAR_ULONG(max_seg_log_file_size, max_seg_log_file_size,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json file size of a segment. If the a segment file large than 1G, the log file will split into piece of segements, each one is 1G. Default size 2G/Segment. ",
        NULL, max_seg_log_file_size_update, _2GB_, 0, ULONG_MAX, 0);

static MYSQL_SYSVAR_BOOL(json_file, json_file_handler_enable,
			 PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json log file Enable|Disable", NULL, json_log_file_enable, 0);

static MYSQL_SYSVAR_BOOL(json_file_flush, json_file_handler_flush,
			 PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_NOCMDOPT,
        "AUDIT plugin json log file flush. Set to ON to perform a flush of the log.", NULL, json_log_file_flush, 0);


static MYSQL_SYSVAR_STR(json_socket_name, json_socket_handler.m_io_dest,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json log unix socket name",
        NULL, json_socket_name_update, "");

static MYSQL_SYSVAR_ULONG(json_socket_write_timeout, json_socket_handler.m_write_timeout,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json socket write timeout, in milliseconds (currently must be at least 1000)",
        NULL, NULL, DEFAULT_WRITE_TIMEOUT,
        0, UINT_MAX32, 0);

static MYSQL_SYSVAR_STR(offsets, offsets_string,
        PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_READONLY  | PLUGIN_VAR_MEMALLOC,
        "AUDIT plugin offsets. Comma separated list of offsets to use for extracting data",
        NULL, NULL, NULL);

static MYSQL_SYSVAR_STR(checksum, checksum_string,
			PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_READONLY  | PLUGIN_VAR_MEMALLOC,
			"AUDIT plugin checksum. Checksum for mysqld corresponding to offsets",
			NULL, NULL, "");

static MYSQL_SYSVAR_STR(password_masking_regex, password_masking_regex_string,
			PLUGIN_VAR_RQCMDARG ,
			"AUDIT plugin regex to use for password masking",
			password_masking_regex_check, password_masking_regex_string_update,
			default_pw_masking_regex
			);

static MYSQL_SYSVAR_BOOL(uninstall_plugin, uninstall_plugin_enable,
        PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_READONLY ,
        "AUDIT uninstall plugin Enable|Disable. Default disabled. If disabled attempts to uninstall the AUDIT plugin via the sql UNINSTALL command will fail.", NULL, NULL, 0);


static MYSQL_SYSVAR_BOOL(offsets_by_version, offsets_by_version_enable,
        PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_READONLY ,
        "AUDIT plugin search offsets by version. If checksum validation doesn't pass will attempt to load and validate offsets according to version. Enable|Disable", NULL, NULL, 1);

static MYSQL_SYSVAR_BOOL(validate_checksum, validate_checksum_enable,
        PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_READONLY ,
        "AUDIT plugin binary checksum validation Enable|Disable", NULL, NULL, 1);

static MYSQL_SYSVAR_BOOL(json_socket, json_socket_handler_enable,
			 PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin json log unix socket Enable|Disable", NULL, json_log_socket_enable, 0);

static MYSQL_SYSVAR_INT(delay_ms, delay_ms_val,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin delay in miliseconds. Delay amount injection. If 0 or negative then delay is disabled.",
        NULL, NULL, 0, 0, INT_MAX32, 0);

static MYSQL_SYSVAR_STR(delay_cmds, delay_cmds_string,
        PLUGIN_VAR_RQCMDARG,
        "AUDIT plugin delay commands to match against comma separated. If empty then delay is disabled.",
			NULL, delay_cmds_string_update, NULL);
static MYSQL_SYSVAR_STR(whitelist_cmds, whitelist_cmds_string,
			PLUGIN_VAR_RQCMDARG,
			"AUDIT plugin whitelisted commands for which queries are not recorded, comma separated",
			NULL, whitelist_cmds_string_update, "BEGIN,COMMIT,PING");
static MYSQL_SYSVAR_STR(record_cmds, record_cmds_string,
			PLUGIN_VAR_RQCMDARG,
			"AUDIT plugin commands for which queries are recorded, comma separated. If set then only queries of these commands will be recorded.",
			NULL, record_cmds_string_update, "SELECT,INSERT,UPDATE,DELETE,SHOW,CREATE,DROP,ALTER");
static MYSQL_SYSVAR_STR(password_masking_cmds, password_masking_cmds_string,
			PLUGIN_VAR_RQCMDARG,
			"AUDIT plugin commands to apply password masking regex to, comma separated",
			NULL, password_masking_cmds_string_update,
			// set password is recorded as set_option
			"CREATE_USER,GRANT,SET_OPTION,SLAVE_START,CREATE_SERVER,ALTER_SERVER,CHANGE_MASTER,UPDATE");
static MYSQL_SYSVAR_STR(whitelist_users, whitelist_users_string,
			PLUGIN_VAR_RQCMDARG,
			"AUDIT plugin whitelisted users whose queries are not recorded, comma separated",
			NULL, whitelist_users_string_update, NULL);

static MYSQL_SYSVAR_STR(record_objs, record_objs_string,
			PLUGIN_VAR_RQCMDARG,
			"AUDIT plugin objects to record, comma separated. If set then only queries containing these objects will be recorded.",
			NULL, record_objs_string_update_extended, NULL);

static MYSQL_SYSVAR_STR(dns_svr_addr, json_formatter.m_dns_svr_addr,
			PLUGIN_VAR_RQCMDARG,
			"Audit the dns svr, which uses for udp server.",
			NULL,dns_svr_addr_update,"");

#if defined (USING_IP_AND_PORT)
static MYSQL_SYSVAR_STR(remote_ip, json_formatter.m_remote_ip,
			PLUGIN_VAR_RQCMDARG,
			"Audit the remote ip, which the audit log sent to.",
			NULL,remote_ip_update,"127.0.0.1");
#endif 
static MYSQL_SYSVAR_UINT(remote_port, json_formatter.m_remote_port,
           PLUGIN_VAR_RQCMDARG,
          "AUDIT the remote port, which the audit log sent to.",
           NULL, remote_port_update, 5000/*default v*/, 0, UINT_MAX32, 0);

static MYSQL_SYSVAR_STR(socket_type, json_formatter.m_socket_type,
            PLUGIN_VAR_RQCMDARG,
            "Audit the remote ip, which the audit log sent to.",
            NULL,socket_type_update,"unix");

static MYSQL_SYSVAR_STR(function_type, function_type_string,
            PLUGIN_VAR_RQCMDARG,
            "To set which function does the plugin support? audit or sql firewall.",
            NULL,function_type_update,"audit");

static MYSQL_SYSVAR_BOOL(parsing_sql, json_formatter.m_do_parsing_sql,
             PLUGIN_VAR_RQCMDARG,
        "Do we parse the sql?. Enable|Disable. Default disabled.", NULL, parsing_sql_enable_func, 0);

static const char *before_after_names[] =
{
	"after", "before", "both", NullS
};


TYPELIB before_after_typelib =
{
	array_elements(before_after_names) - 1,
	"before_after_typelib",
	before_after_names,
	NULL
};

static MYSQL_SYSVAR_ENUM(before_after, before_after_mode,
        PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_MEMALLOC,
        "AUDIT plugin log statements before execution, after, or both. Default is 'after'",
	NULL, NULL, 0,
	& before_after_typelib);

/*
 * Plugin system vars
 */
static struct st_mysql_sys_var* audit_system_variables[] =
{
#ifdef HAVE_SESS_CONNECT_ATTRS
	MYSQL_SYSVAR(sess_connect_attrs),
#endif	
	MYSQL_SYSVAR(socket_creds),
	MYSQL_SYSVAR(client_capabilities),
	MYSQL_SYSVAR(header_msg),
	MYSQL_SYSVAR(force_record_logins),
	MYSQL_SYSVAR(json_log_file),
	MYSQL_SYSVAR(json_file_bufsize),
	MYSQL_SYSVAR(json_file_sync),
	MYSQL_SYSVAR(json_file_retry),
	MYSQL_SYSVAR(json_socket_retry),
	MYSQL_SYSVAR(json_file),
	MYSQL_SYSVAR(json_file_flush),
	MYSQL_SYSVAR(uninstall_plugin),
	MYSQL_SYSVAR(validate_checksum),
	MYSQL_SYSVAR(offsets_by_version),
	MYSQL_SYSVAR(json_socket_name),
	MYSQL_SYSVAR(offsets),
	MYSQL_SYSVAR(json_socket),
	MYSQL_SYSVAR(query_cache_table_list),
	MYSQL_SYSVAR(is_thd_printed_list),
	MYSQL_SYSVAR(delay_ms),
	MYSQL_SYSVAR(delay_cmds),
	MYSQL_SYSVAR(whitelist_cmds),
	MYSQL_SYSVAR(record_cmds),
	MYSQL_SYSVAR(password_masking_cmds),
	MYSQL_SYSVAR(whitelist_users),
	MYSQL_SYSVAR(record_objs),
	MYSQL_SYSVAR(checksum),
	MYSQL_SYSVAR(password_masking_regex),
	MYSQL_SYSVAR(set_peer_cred),
	MYSQL_SYSVAR(peer_is_uds),
	MYSQL_SYSVAR(peer_info),
	MYSQL_SYSVAR(before_after),
	MYSQL_SYSVAR(json_socket_write_timeout),
	MYSQL_SYSVAR(max_seg_log_file_size),
#if defined (USING_IP_AND_PORT)
	MYSQL_SYSVAR(remote_ip),
#endif 
	MYSQL_SYSVAR(remote_port),
	MYSQL_SYSVAR(socket_type),
	MYSQL_SYSVAR(dns_svr_addr),
	MYSQL_SYSVAR(function_type),
	MYSQL_SYSVAR(parsing_sql),
	NULL
};

// declare our plugin
mysql_declare_plugin(audit_plugin)
{
	plugin_type,
	&audit_plugin,
	"AUDIT",
	"QiYi Inc.",
	"AUDIT plugin, creates a file mysql-audit.log to log activity",
	PLUGIN_LICENSE_GPL,
	audit_plugin_init, /* Plugin Init */
	audit_plugin_deinit, /* Plugin Deinit */
	0x0100 /* 1.0 */,
	audit_status, /* status variables*/
	audit_system_variables, /* system variables*/
	NULL /* config options*/
}
mysql_declare_plugin_end;

#if MYSQL_VERSION_ID < 50505
/**
 * DLL constructor method.
 * We set here the audit plugin version to the same as the first built in plugin.
 * This is so we can have a single lib for all versions (needed in 5.1)
 */
extern "C" void __attribute__ ((constructor)) audit_plugin_so_init(void)
{
	if (mysqld_builtins && mysqld_builtins[0])
	{
		audit_plugin.interface_version = *(int *) mysqld_builtins[0]->info;
		sql_print_information(get_text("%s Set interface version to: %d (%d)"),
				log_prefix, audit_plugin.interface_version,
				audit_plugin.interface_version >> 8);
	}
	else
	{
		sql_print_error(
				get_text("%s mysqld_builtins are null. Plugin will not load unless the mysql version is: %d. \n"),
				log_prefix, audit_plugin.interface_version >> 8);
	}

	memset(peer_info_init_value, '0', sizeof(peer_info_init_value)-1);
	peer_info_init_value[sizeof(peer_info_init_value) - 1] = '\0';
}
#elif MYSQL_VERSION_ID < 50600
extern struct st_mysql_plugin *mysql_mandatory_plugins[];
extern "C"  void __attribute__ ((constructor)) audit_plugin_so_init(void)
{
	audit_plugin.interface_version = *(int *) mysql_mandatory_plugins[0]->info;
	sql_print_information(get_text("%s Set interface version to: %d (%d)"),
			log_prefix, audit_plugin.interface_version,
			audit_plugin.interface_version >> 8);

	memset(peer_info_init_value, '0', sizeof(peer_info_init_value)-1);
	peer_info_init_value[sizeof(peer_info_init_value) - 1] = '\0';
}
#elif !defined(MARIADB_BASE_VERSION) && MYSQL_VERSION_ID < 50709
// Interface version for MySQL 5.6 changed in 5.6.14.
// This is not needed for 5.7.
extern "C"  void __attribute__ ((constructor)) audit_plugin_so_init(void)
{
	const char *ver_5_6_13 = "5.6.13";
	if (strncmp(server_version, ver_5_6_13, strlen(ver_5_6_13)) <= 0)
	{
		audit_plugin.interface_version = 0x0300;
	}
	else
	{
		audit_plugin.interface_version = 0x0301;
	}

	memset(peer_info_init_value, '0', sizeof(peer_info_init_value)-1);
	peer_info_init_value[sizeof(peer_info_init_value) - 1] = '\0';
}
#else
// 5.7
extern "C"  void __attribute__ ((constructor)) audit_plugin_so_init(void)
{
	memset(peer_info_init_value, '0', sizeof(peer_info_init_value)-1);
	peer_info_init_value[sizeof(peer_info_init_value) - 1] = '\0';
}
#endif

/*
 * Pure virtual handler. Needed when running in mysql compiled with a newer version of gcc.
 * Versions of mysql for RH 6 and Percona this function is defined local in mysqld.
 * So we define our own implementation.
 */
extern "C" int __cxa_pure_virtual (void)
{
	sql_print_error(
			"%s __cxa_pure_virtual called. Fatal condition. ",
			log_prefix);
	return 0;
}

/*
 * Variable to hold version
 */
MYSQL_AUDIT_PLUGIN_SYMBOL_VERSION() = '\0';
