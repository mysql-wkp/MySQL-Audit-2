Some changes are listed as following:

(1) Adds the golba variables "audit_max_seg_file_size". which descibes the size of a segment file. the previous version, the plugin does not support to slice the file when the audit.log file exceeds over 2GB. Therefore, a global variable "audit_max_seg_file_size" is added to indicate. 

(2) we change the log file name from audit.log to audit_YYYY_MM_DD_xxxxxxxxxxxxserial_number.log. Here, YYYY is year, MM means month, the days is described by DD. 
xxxxxxxxxxxxserial_number is a  series number, its increament is 1. If a slice of log file is over 2GB, the series number will added by 1. After runing a long time, weperhaps get a log of slice files, which each one has 2GB size. 
For example:
audit_2017_06_23_000000000000000001.log,
audit_2017_06_23_000000000000000002.log,
...
audit_2017_06_24_000000000000000001.log


