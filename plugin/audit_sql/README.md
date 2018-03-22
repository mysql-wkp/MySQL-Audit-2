AUDIT Plugin for MySQL<sup>*</sup>
===================

A MySQL plugin from QiYi providing audit capabilities for MySQL, 
designed with an emphasis on security and audit requirements. The plugin may be used 
as a standalone audit solution or configured to feed data to external monitoring tools.


Installation and Configuration 
------------------------------

Please check out our wiki on github for detailed installation and configuration instructions:

http://gitlab.qiyi.domain/database/MySQL-Audit/wikis/home

Issues
------------------------------

Found a bug? Got a feature request or question?

Please feel free to report to:http://gitlab.qiyi.domain/database/MySQL-Audit/issues 

If reporting a bug, please describe the problem verbosely. Try to see if it reproduces and 
include a detailed description on how to reproduce.
 
Make sure to include your MySQL Server version and Audit Plugin version.
To print MySQL Server version: log into MySQL and execute the command: 

    status

Please include with the bug the MySQL error log. 
Log file location can be queried by running the following command: 

     show global variables like 'log_error'


Source Code
-------------------------------
Source code is available at: http://gitlab.qiyi.domain/database/MySQL-Audit/tree/master

Lex/Bison

To generate the lexical/grammatical file. using the folloing comamnd.

grammatical: bison --yacc --output=grammar.cpp --defines=grammar.h grammar.y

lexical: flex --outfile=scanner.cpp --header-file=scanner.h    scanner.l	
