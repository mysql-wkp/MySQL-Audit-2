/*
*  A new query engines for Transformer.
*  path:query/parser/parser.h
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include <types.h>
#include <nodes.h>
#include <system.h>
#include <scannerGramm.h>
#include <grammar.h>
#include <parserGramm.h>


class TRANS_EXPORT Parser : public NonCopyable {
public:
	Parser () ;
	~Parser() ;
	virtual ASTNode* raw_parser(const char* str); 	
    virtual void init_parser(TransYY_yy_extra_type* yy_extra_data) ;

	static int TransYY_yylex(YYSTYPE *lvalp, YYLTYPE *llocp, Trans_yyscan_t yyscanner) ;
} ;


#endif   /* __PARSER_H__ */
