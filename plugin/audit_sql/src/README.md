/**************************************************************************************************************************

If you modify the .y or .l file, the following command should be run, and copy the .h file to include directory.

Thaks.

**************************************************************************************************************************/

//For the .y to generate grammar.cpp and grammar.h

bison --yacc --output=grammar.cpp --defines=grammar.h grammar.y

//For the .l to generate scanner.cpp and sacnner.h

flex --outfile=scanner.cpp --header-file=scanner.h    scanner.l
