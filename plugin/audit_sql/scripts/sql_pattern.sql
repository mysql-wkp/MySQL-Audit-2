--if the initialization failed, we can use this script to create sql_pattern table manually.
use mysql;

CREATE TABLE IF NOT EXISTS sql_pattern ( sql_text varchar(1024) DEFAULT '' NOT NULL, sql_pattern varchar(1024) DEFAULT '' NOT NULL, sql_md5 varchar(512) DEFAULT '' NOT NULL) engine=InnoDB STATS_PERSISTENT=0 CHARACTER SET utf8 COLLATE utf8_general_ci comment='MySQL sql patterns';

