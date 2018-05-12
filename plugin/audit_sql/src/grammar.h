
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENT = 258,
     FCONST = 259,
     SCONST = 260,
     BCONST = 261,
     XCONST = 262,
     Op = 263,
     ICONST = 264,
     PARAM = 265,
     TYPECAST = 266,
     DOT_DOT = 267,
     COLON_EQUALS = 268,
     EQUALS_GREATER = 269,
     LESS_EQUALS = 270,
     GREATER_EQUALS = 271,
     NOT_EQUALS = 272,
     CROSS = 273,
     NULLS_P = 274,
     NOT = 275,
     WITH = 276,
     BEGIN_P = 277,
     BETWEEN = 278,
     IN_P = 279,
     IS = 280,
     IS_P = 281,
     INNER = 282,
     LIKE = 283,
     ILIKE = 284,
     SIMILAR = 285,
     NOT_LA = 286,
     FIRST_P = 287,
     LAST_P = 288,
     LIMIT = 289,
     TIME = 290,
     ORDINALITY = 291,
     NULLS_LA = 292,
     NULL_P = 293,
     AS = 294,
     TEMPORARY = 295,
     TEMP = 296,
     INTO = 297,
     LOCAL = 298,
     UNLOGGED = 299,
     TABLE = 300,
     ALL = 301,
     GLOBAL = 302,
     BY = 303,
     GROUP_P = 304,
     ABSOLUTE_P = 305,
     ABORT_P = 306,
     AND = 307,
     ON = 308,
     OR = 309,
     DROP = 310,
     ALTER = 311,
     ADD_P = 312,
     COLUMN = 313,
     DISTINCT = 314,
     SUM = 315,
     AVG = 316,
     ORDER_P = 317,
     HAVING = 318,
     UPDATE = 319,
     SET = 320,
     VALUES = 321,
     INSERT = 322,
     OF_P = 323,
     CURRENT_P = 324,
     DELETE = 325,
     DATABASE = 326,
     ROLE = 327,
     CREATE = 328,
     RULE = 329,
     IGNORE = 330,
     KILLSESSION = 331,
     COMMIT_P = 332,
     ROLLBACK_P = 333,
     KEY_P = 334,
     FOREIGN = 335,
     PRIMARY = 336,
     CONSTRAINT = 337,
     REFERENCES = 338,
     USER = 339,
     JOIN = 340,
     NATURE = 341,
     NOTNULL = 342,
     FULL = 343,
     LEFT = 344,
     RIGHT = 345,
     USING = 346,
     SELECT = 347,
     FROM = 348,
     WHERE = 349,
     UMINUS = 350
   };
#endif
/* Tokens.  */
#define IDENT 258
#define FCONST 259
#define SCONST 260
#define BCONST 261
#define XCONST 262
#define Op 263
#define ICONST 264
#define PARAM 265
#define TYPECAST 266
#define DOT_DOT 267
#define COLON_EQUALS 268
#define EQUALS_GREATER 269
#define LESS_EQUALS 270
#define GREATER_EQUALS 271
#define NOT_EQUALS 272
#define CROSS 273
#define NULLS_P 274
#define NOT 275
#define WITH 276
#define BEGIN_P 277
#define BETWEEN 278
#define IN_P 279
#define IS 280
#define IS_P 281
#define INNER 282
#define LIKE 283
#define ILIKE 284
#define SIMILAR 285
#define NOT_LA 286
#define FIRST_P 287
#define LAST_P 288
#define LIMIT 289
#define TIME 290
#define ORDINALITY 291
#define NULLS_LA 292
#define NULL_P 293
#define AS 294
#define TEMPORARY 295
#define TEMP 296
#define INTO 297
#define LOCAL 298
#define UNLOGGED 299
#define TABLE 300
#define ALL 301
#define GLOBAL 302
#define BY 303
#define GROUP_P 304
#define ABSOLUTE_P 305
#define ABORT_P 306
#define AND 307
#define ON 308
#define OR 309
#define DROP 310
#define ALTER 311
#define ADD_P 312
#define COLUMN 313
#define DISTINCT 314
#define SUM 315
#define AVG 316
#define ORDER_P 317
#define HAVING 318
#define UPDATE 319
#define SET 320
#define VALUES 321
#define INSERT 322
#define OF_P 323
#define CURRENT_P 324
#define DELETE 325
#define DATABASE 326
#define ROLE 327
#define CREATE 328
#define RULE 329
#define IGNORE 330
#define KILLSESSION 331
#define COMMIT_P 332
#define ROLLBACK_P 333
#define KEY_P 334
#define FOREIGN 335
#define PRIMARY 336
#define CONSTRAINT 337
#define REFERENCES 338
#define USER 339
#define JOIN 340
#define NATURE 341
#define NOTNULL 342
#define FULL 343
#define LEFT 344
#define RIGHT 345
#define USING 346
#define SELECT 347
#define FROM 348
#define WHERE 349
#define UMINUS 350




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 45 "grammar.y"

	/*The type of bison*/
	Trans_YYSTYPE	_trans_yystype_;
	
	int 			_ival_;
	char* 			_str_;
	const char* 	_keyword_;
	bool			_boolean_;
	
	ASTNode*		_astNode_;
	TargetList*     _targetList_;
	TargetEntry*    _targetEntry_; 
	TableEntry*     _tableEntry_; 
	TableEntryList* _tableList_;
	FromStmt* 	    _fromStmt_;
	WhereStmt*      _whereStmt_;
	
	AlterOptList*	_alterOptionList_;
	AlterOpt* 		_alterOpt_;	
	Expr*			_expr_;
	ExprList*       _exprList_;
	A_ExprList*       _A_exprList_;
			
	Function*      _function_;
	FunctionArgList* _funcArgList_;
	AggregateStmt*   _aggrStmt_;

	JoinedTypes		_join_type_;



/* Line 1676 of yacc.c  */
#line 274 "grammar.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



