Some changes are listed as following:

(1) Adds the golba variables "audit_max_seg_file_size". which descibes the size of a segment file. the previous version, the plugin does not support to slice the file when the audit.log file exceeds over 2GB. Therefore, a global variable "audit_max_seg_file_size" is added to indicate. 

(2) we change the log file name from audit.log to audit_YYYY_MM_DD_xxxxxxxxxxxxserial_number.log. Here, YYYY is year, MM means month, the days is described by DD. 
xxxxxxxxxxxxserial_number is a  series number, its increament is 1. If a slice of log file is over 2GB, the series number will added by 1. After runing a long time, weperhaps get a log of slice files, which each one has 2GB size. 
For example:
audit_2017_06_23_000000000000000001.log,
audit_2017_06_23_000000000000000002.log,
...
audit_2017_06_24_000000000000000001.log

A new catalog which name is 'sql_pattern', is created when db is doing boostrup. the srcipt of creating is in scripts forlder. There tow senariors: (1) the db instance is running already. we use the table creating script and run it in mysql console; (2)A new db instance, the catalog table will be created automatically. 

When plugin run as firewall mode, all rules are all in sql_pattern. When db is starting, the rules read from sql_pattern will be loaded into a cache. Users execute a sql statment, a pattern of this sql statment is parsed, then do action described in sql_pattern; 
