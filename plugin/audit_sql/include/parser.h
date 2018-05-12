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
	virtual ~Parser() ;
	virtual ASTNode* raw_parser(const char* str); 	
	virtual ASTNode* raw_parser2 (const char* str);
	
	virtual void init_parser();
    virtual void init_parser(const char* str) ;
    virtual void init_parser(TransYY_yy_extra_type* yy_extra_data) ;

	static int TransYY_yylex(YYSTYPE *lvalp, YYLTYPE *llocp, Trans_yyscan_t yyscanner) ;
	virtual void finish_parser () ;
private:
	Trans_yyscan_t m_lex_scanner_ ;
	TransYY_yy_extra_type m_bison_yy_;
} ;


#endif   /* __PARSER_H__ */
