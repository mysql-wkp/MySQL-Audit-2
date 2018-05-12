
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse         TransYY_yyparse
#define yylex           TransYY_yylex
#define yyerror         TransYY_yyerror
#define yylval          TransYY_yylval
#define yychar          TransYY_yychar
#define yydebug         TransYY_yydebug
#define yynerrs         TransYY_yynerrs
#define yylloc          TransYY_yylloc

/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "grammar.y"

/*****************************************************************************
*     the SQL grammay part.
*
******************************************************************************/
#include <helper.h>
#include <nodes.h>

#include <scannerGramm.h>
#include <scanner.h>

#include <parser.h>
#include <parserGramm.h>

#define YYLLOC_DEFAULT(Current, Rhs, N) \
	do { \
		if ((N) > 0) \
			(Current) = (Rhs)[1]; \
		else \
			(Current) = (-1); \
	} while (0)

#define parser_yyerror(msg)  Trans_yyerror(msg, yyscanner)

static void TransYY_yyerror(YYLTYPE *yylloc, Trans_yyscan_t yyscanner,
						 const char *msg);
#if defined yylex
#undef yylex 
#define yylex Parser::TransYY_yylex
#endif


/* Line 189 of yacc.c  */
#line 114 "grammar.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 372 "grammar.cpp"
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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 397 "grammar.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  57
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   498

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  153
/* YYNRULES -- Number of states.  */
#define YYNSTATES  291

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   350

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   108,     2,     2,     2,     2,     2,
      97,    98,   100,   103,    99,   104,   101,   105,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    95,
     106,   102,   107,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      96
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    11,    13,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    32,    34,    36,    40,
      44,    46,    48,    54,    57,    58,    60,    62,    63,    65,
      69,    71,    75,    81,    83,    87,    90,    91,    95,    97,
     101,   103,   105,   109,   115,   117,   121,   127,   131,   137,
     139,   141,   143,   145,   147,   150,   155,   158,   163,   168,
     174,   179,   185,   191,   197,   200,   201,   203,   208,   212,
     213,   217,   218,   221,   222,   225,   226,   228,   232,   236,
     239,   243,   247,   251,   255,   259,   263,   267,   271,   275,
     279,   283,   286,   290,   295,   301,   307,   311,   316,   319,
     323,   325,   327,   331,   333,   335,   337,   339,   341,   345,
     348,   350,   352,   356,   361,   366,   369,   373,   377,   379,
     381,   386,   390,   392,   395,   399,   406,   409,   413,   418,
     423,   427,   434,   436,   440,   444,   448,   451,   456,   457,
     462,   466,   469,   472,   476,   480,   484,   486,   488,   490,
     498,   506,   510,   512
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     110,     0,    -1,   111,    -1,   111,    95,   112,    -1,   112,
      -1,   113,    -1,   144,    -1,   147,    -1,   151,    -1,   152,
      -1,   155,    -1,   156,    -1,    22,    -1,    77,    -1,    78,
      -1,    -1,   115,    -1,   114,    -1,    97,   115,    98,    -1,
      97,   114,    98,    -1,   116,    -1,   117,    -1,    92,   118,
     122,   129,   130,    -1,   119,   120,    -1,    -1,    59,    -1,
      46,    -1,    -1,   121,    -1,   120,    99,   121,    -1,   139,
      -1,    97,   139,    98,    -1,    97,   139,    98,    39,   139,
      -1,   100,    -1,   139,    39,   139,    -1,    93,   123,    -1,
      -1,   123,    99,   124,    -1,   124,    -1,    97,   124,    98,
      -1,   125,    -1,   128,    -1,    97,   128,    98,    -1,    97,
     128,    98,    39,     3,    -1,     3,    -1,     3,   101,     3,
      -1,     3,   101,     3,   101,     3,    -1,     3,    39,     3,
      -1,     3,   101,     3,    39,     3,    -1,    89,    -1,    90,
      -1,    88,    -1,    27,    -1,    86,    -1,    53,   137,    -1,
      53,    97,   137,    98,    -1,    91,   137,    -1,    91,    97,
     137,    98,    -1,   125,    18,    85,   125,    -1,   125,   126,
      85,   125,   127,    -1,   125,    85,   125,   127,    -1,   128,
     126,    85,   128,   127,    -1,   128,   126,    85,   125,   127,
      -1,   125,   126,    85,   128,   127,    -1,    94,   136,    -1,
      -1,   131,    -1,   133,   134,   132,   135,    -1,    62,    48,
     139,    -1,    -1,    49,    48,   139,    -1,    -1,    63,   139,
      -1,    -1,    34,   139,    -1,    -1,   137,    -1,   136,    52,
     136,    -1,   136,    54,   136,    -1,    20,   136,    -1,    97,
     136,    98,    -1,   139,   102,   139,    -1,   139,   103,   139,
      -1,   139,   104,   139,    -1,   139,   100,   139,    -1,   139,
     105,   139,    -1,   139,   106,   139,    -1,   139,   107,   139,
      -1,   139,    15,   139,    -1,   139,    16,   139,    -1,   139,
      17,   139,    -1,    31,   139,    -1,   139,    28,   139,    -1,
     139,    31,    28,   139,    -1,   139,    23,   139,    52,   139,
      -1,   139,    24,    97,   138,    98,    -1,   139,    25,    38,
      -1,   139,    25,    20,    38,    -1,   139,    87,    -1,   138,
      99,   139,    -1,   139,    -1,     3,    -1,     3,   101,     3,
      -1,     9,    -1,     4,    -1,   140,    -1,     6,    -1,     7,
      -1,   108,     3,   108,    -1,     8,     3,    -1,   141,    -1,
       5,    -1,     3,    97,    98,    -1,     3,    97,   142,    98,
      -1,     3,    97,   100,    98,    -1,    97,    98,    -1,    97,
     142,    98,    -1,   142,    99,   143,    -1,   143,    -1,   139,
      -1,    56,    45,   139,   145,    -1,   145,    99,   146,    -1,
     146,    -1,    57,   139,    -1,    57,    58,   139,    -1,    57,
      80,    79,   139,    83,   139,    -1,    55,   139,    -1,    55,
      58,   139,    -1,    55,    80,    79,   139,    -1,    55,    81,
      79,   139,    -1,    55,    82,   139,    -1,    64,   139,    65,
     148,   122,   150,    -1,   149,    -1,   148,    99,   149,    -1,
       3,   102,   137,    -1,     3,   102,   139,    -1,    94,   136,
      -1,    94,    69,    68,   139,    -1,    -1,    70,    93,   139,
     129,    -1,    67,   153,   154,    -1,    42,   139,    -1,    66,
     138,    -1,    55,    71,   139,    -1,    55,    84,   139,    -1,
      55,    72,   139,    -1,   157,    -1,   158,    -1,   159,    -1,
      73,    74,   139,    39,   112,    21,   160,    -1,    64,    74,
     139,    39,   112,    21,   160,    -1,    55,    74,   139,    -1,
      75,    -1,    76,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   120,   120,   125,   129,   135,   138,   141,   144,   147,
     150,   153,   156,   160,   164,   168,   172,   173,   177,   178,
     182,   188,   191,   207,   208,   211,   212,   213,   216,   222,
     235,   239,   243,   247,   252,   259,   264,   270,   280,   285,
     292,   295,   298,   301,   313,   319,   329,   340,   349,   363,
     366,   369,   372,   375,   380,   384,   388,   392,   398,   403,
     409,   415,   421,   427,   435,   441,   445,   451,   465,   469,
     474,   478,   483,   487,   492,   496,   501,   502,   508,   514,
     519,   525,   530,   535,   540,   545,   550,   555,   560,   565,
     570,   582,   587,   592,   597,   602,   607,   612,   617,   625,
     635,   643,   648,   656,   660,   665,   670,   675,   679,   684,
     692,   697,   702,   708,   714,   724,   730,   738,   748,   755,
     759,   765,   770,   777,   781,   785,   789,   793,   797,   801,
     805,   813,   824,   829,   841,   849,   858,   863,   868,   873,
     881,   890,   897,   903,   907,   911,   918,   919,   920,   924,
     931,   938,   944,   948
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENT", "FCONST", "SCONST", "BCONST",
  "XCONST", "Op", "ICONST", "PARAM", "TYPECAST", "DOT_DOT", "COLON_EQUALS",
  "EQUALS_GREATER", "LESS_EQUALS", "GREATER_EQUALS", "NOT_EQUALS", "CROSS",
  "NULLS_P", "NOT", "WITH", "BEGIN_P", "BETWEEN", "IN_P", "IS", "IS_P",
  "INNER", "LIKE", "ILIKE", "SIMILAR", "NOT_LA", "FIRST_P", "LAST_P",
  "LIMIT", "TIME", "ORDINALITY", "NULLS_LA", "NULL_P", "AS", "TEMPORARY",
  "TEMP", "INTO", "LOCAL", "UNLOGGED", "TABLE", "ALL", "GLOBAL", "BY",
  "GROUP_P", "ABSOLUTE_P", "ABORT_P", "AND", "ON", "OR", "DROP", "ALTER",
  "ADD_P", "COLUMN", "DISTINCT", "SUM", "AVG", "ORDER_P", "HAVING",
  "UPDATE", "SET", "VALUES", "INSERT", "OF_P", "CURRENT_P", "DELETE",
  "DATABASE", "ROLE", "CREATE", "RULE", "IGNORE", "KILLSESSION",
  "COMMIT_P", "ROLLBACK_P", "KEY_P", "FOREIGN", "PRIMARY", "CONSTRAINT",
  "REFERENCES", "USER", "JOIN", "NATURE", "NOTNULL", "FULL", "LEFT",
  "RIGHT", "USING", "SELECT", "FROM", "WHERE", "';'", "UMINUS", "'('",
  "')'", "','", "'*'", "'.'", "'='", "'+'", "'-'", "'/'", "'<'", "'>'",
  "'\"'", "$accept", "stmtblock", "stmtmulti", "stmt", "SelectStmt",
  "select_with_parens", "select_no_parens", "select_clause",
  "simple_select", "target_list_opt", "target_opt", "target_list_stmt",
  "target_entry", "from_stmt", "from_list", "tables_ref", "table_entry",
  "joined_type", "joined_qual", "joined_table", "where_stmt",
  "aggregate_stmt", "aggregate_expr", "orderby_expr", "groupby_expr",
  "having_expr", "limit_expr", "condition_expr", "compare_expr",
  "a_expr_list", "a_expr", "Sconst", "func_expr", "func_arg_list",
  "func_arg_expr", "AlterStmt", "AlterStmt_Opts", "AlterStmt_Opt",
  "UpdateStmt", "set_expr_list", "set_clause", "where_current_stmt",
  "DeleteStmt", "InsertStmt", "insert_target_stmt", "insert_values_stmt",
  "DropStmt", "SQLRuleStmt", "CreateRuleStmt", "UpdateRuleStmt",
  "DropRuleStmt", "ActionOptStmt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,    59,   350,    40,    41,    44,
      42,    46,    61,    43,    45,    47,    60,    62,    34
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   109,   110,   111,   111,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   113,   113,   114,   114,
     115,   116,   117,   118,   118,   119,   119,   119,   120,   120,
     121,   121,   121,   121,   121,   122,   122,   123,   123,   123,
     124,   124,   124,   124,   125,   125,   125,   125,   125,   126,
     126,   126,   126,   126,   127,   127,   127,   127,   128,   128,
     128,   128,   128,   128,   129,   129,   130,   131,   132,   132,
     133,   133,   134,   134,   135,   135,   136,   136,   136,   136,
     136,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   138,
     138,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   140,   141,   141,   141,   141,   141,   142,   142,   143,
     144,   145,   145,   146,   146,   146,   146,   146,   146,   146,
     146,   147,   148,   148,   149,   149,   150,   150,   150,   151,
     152,   153,   154,   155,   155,   155,   156,   156,   156,   157,
     158,   159,   160,   160
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     3,     3,
       1,     1,     5,     2,     0,     1,     1,     0,     1,     3,
       1,     3,     5,     1,     3,     2,     0,     3,     1,     3,
       1,     1,     3,     5,     1,     3,     5,     3,     5,     1,
       1,     1,     1,     1,     2,     4,     2,     4,     4,     5,
       4,     5,     5,     5,     2,     0,     1,     4,     3,     0,
       3,     0,     2,     0,     2,     0,     1,     3,     3,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     3,     4,     5,     5,     3,     4,     2,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     3,     2,
       1,     1,     3,     4,     4,     2,     3,     3,     1,     1,
       4,     3,     1,     2,     3,     6,     2,     3,     4,     4,
       3,     6,     1,     3,     3,     3,     2,     4,     0,     4,
       3,     2,     2,     3,     3,     3,     1,     1,     1,     7,
       7,     3,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      15,    12,     0,     0,     0,     0,     0,     0,    13,    14,
      24,     0,     0,     2,     4,     5,    17,    16,    20,    21,
       6,     7,     8,     9,    10,    11,   146,   147,   148,     0,
       0,     0,     0,     0,   101,   104,   111,   106,   107,     0,
     103,     0,     0,     0,     0,   105,   110,     0,     0,     0,
       0,    26,    25,    36,     0,     0,     0,     1,    15,   143,
     145,   151,   144,     0,     0,     0,   109,     0,   115,   119,
       0,   118,     0,     0,   141,     0,   140,    65,     0,     0,
      65,     0,    33,    23,    28,    30,    19,    18,     3,     0,
       0,   120,   122,   112,     0,     0,   102,    15,   116,     0,
     108,     0,    36,   132,   142,   100,     0,   139,    15,    44,
       0,    35,    38,    40,    41,    71,   119,     0,     0,     0,
       0,     0,     0,   126,     0,     0,   123,     0,   114,   113,
       0,   117,     0,     0,   138,     0,     0,     0,     0,    64,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,     0,    53,    51,    49,    50,     0,     0,     0,    22,
      66,    73,    31,    29,    34,   127,     0,     0,   130,   124,
       0,   121,     0,   134,   135,   133,     0,   131,    99,    79,
      91,     0,   119,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    45,     0,     0,    39,    42,    37,     0,
       0,     0,     0,     0,     0,    69,     0,   128,   129,     0,
     152,   153,   150,     0,   136,    80,    77,    78,    88,    89,
      90,     0,     0,     0,    96,    92,     0,    84,    81,    82,
      83,    85,    86,    87,   149,     0,     0,     0,    58,     0,
       0,    60,     0,     0,     0,     0,    70,    72,     0,    75,
      32,     0,     0,     0,     0,    97,    93,    48,    46,    43,
       0,    54,     0,    56,    59,    63,    62,    61,     0,     0,
      67,   125,   137,    94,    95,     0,     0,    68,    74,    55,
      57
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    17,    18,    19,    53,
      54,    83,    84,    80,   111,   112,   113,   157,   251,   114,
     107,   159,   160,   259,   161,   215,   280,   139,   140,   104,
     141,    45,    46,    70,    71,    20,    91,    92,    21,   102,
     103,   177,    22,    23,    48,    76,    24,    25,    26,    27,
      28,   222
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -128
static const yytype_int16 yypact[] =
{
     366,  -128,   179,     4,   275,    -3,   -42,    10,  -128,  -128,
      50,   -74,    67,    -8,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,   390,
     390,   390,   390,   390,     3,  -128,  -128,  -128,  -128,    96,
    -128,   390,   282,   103,    47,  -128,  -128,   390,    51,   390,
     390,  -128,  -128,    26,   292,    23,    28,  -128,   366,  -128,
    -128,  -128,  -128,   -36,    27,   129,  -128,    98,  -128,  -128,
     -23,  -128,    36,   143,  -128,   390,  -128,    65,   121,     0,
      65,   282,  -128,    62,  -128,   125,  -128,  -128,  -128,     8,
     197,    66,  -128,  -128,    69,    -5,  -128,   366,  -128,   390,
    -128,    68,   -19,  -128,    70,  -128,   210,  -128,   366,   -32,
       1,    79,  -128,   234,    -7,   131,    87,   292,   390,   390,
     107,   110,   390,  -128,   390,   118,  -128,   -36,  -128,  -128,
     177,  -128,   323,   143,   113,   390,   210,   390,   148,   -12,
    -128,   350,   190,   224,   226,   228,   135,   -25,     1,   150,
    -128,   228,  -128,  -128,  -128,  -128,   151,   153,   191,  -128,
    -128,   180,   201,  -128,  -128,  -128,   390,   390,  -128,  -128,
     390,  -128,    35,  -128,   350,  -128,   168,  -128,  -128,   -12,
    -128,   -30,   350,   210,   210,   390,   390,   390,   390,   145,
      24,   390,   216,  -128,   390,   390,   390,   390,   390,   390,
     390,    35,  -128,   -31,   234,   -25,  -128,   208,  -128,   228,
     -44,   228,   228,   390,   390,   187,   390,  -128,  -128,   171,
    -128,  -128,  -128,   192,   -12,  -128,   219,  -128,  -128,  -128,
    -128,   212,   390,   254,  -128,  -128,   390,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,   259,   290,   299,  -128,   330,
     337,  -128,   387,   181,   387,   181,  -128,  -128,   255,   270,
    -128,   390,   390,   390,    31,  -128,  -128,  -128,  -128,  -128,
     217,  -128,   217,  -128,  -128,  -128,  -128,  -128,   390,   390,
    -128,  -128,  -128,  -128,  -128,   214,   215,  -128,  -128,  -128,
    -128
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -128,  -128,  -128,    -6,  -128,   295,   305,  -128,  -128,  -128,
    -128,  -128,   200,   245,  -128,  -100,   -73,  -112,    56,  -104,
     241,  -128,  -128,  -128,  -128,  -128,  -128,   -35,  -127,   116,
      -4,  -128,  -128,   286,   252,  -128,  -128,   225,  -128,  -128,
     220,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,   154
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -28
static const yytype_int16 yytable[] =
{
      44,   156,   150,   109,   109,   173,   147,   143,   245,   249,
     146,    34,    35,    36,    37,    38,    39,    40,    10,    89,
     150,    90,   183,    11,   184,    59,    60,    61,    62,    63,
      34,    35,    36,    37,    38,    39,    40,    67,    69,    47,
     183,   205,   184,    74,   233,    77,    78,   250,   208,    33,
      85,    49,    88,   -27,   -27,   -27,   -27,   -27,   -27,   -27,
      69,   152,   234,   153,   154,   155,   119,    57,   225,   144,
     246,   105,   204,   207,    79,    98,    99,   116,   210,   152,
     133,   153,   154,   155,    50,   123,   126,    58,   120,   121,
     122,   130,   156,   129,    99,    69,    51,   110,   145,    66,
      64,   179,   142,   181,    65,    42,    72,   253,   255,    52,
     220,   221,    73,    85,   164,   165,    43,    75,   168,    79,
     169,    86,   271,   273,    42,    93,    87,    94,   174,   284,
     135,   178,    96,   180,   182,    43,   248,    97,   252,   254,
     156,   224,   156,   285,   100,   286,   101,   -27,   226,   227,
     -27,    34,    35,    36,    37,    38,    39,    40,   -27,   106,
     108,   117,   217,   218,   118,   127,   219,   128,   136,   135,
     132,    34,    35,    36,    37,    38,    39,    40,   148,   137,
     158,   228,   229,   230,   231,   162,   166,   235,   136,   167,
     237,   238,   239,   240,   241,   242,   243,   170,   172,   137,
      34,    35,    36,    37,    38,    39,    40,   176,   150,   256,
     257,   201,   260,    34,    35,    36,    37,    38,    39,    40,
      34,    35,    36,    37,    38,    39,    40,   202,   105,   203,
     136,   109,   266,   206,   249,   209,   211,   223,   212,   213,
     216,   137,   232,   214,   236,   138,    68,   247,   137,   258,
      29,    30,   149,    31,   261,   124,    43,   281,   282,   283,
     262,   150,   267,    32,   263,   138,   182,   152,   182,   153,
     154,   155,   250,   184,   287,   288,    43,   125,    34,    35,
      36,    37,    38,    39,    40,    34,    35,    36,    37,    38,
      39,    40,   265,   268,    42,    34,    35,    36,    37,    38,
      39,    40,   269,   278,   279,    43,    55,   138,   274,   275,
     276,   277,   289,   290,    42,    68,    56,   163,    43,   151,
     152,   115,   153,   154,   155,    43,    34,    35,    36,    37,
      38,    39,    40,    34,    35,    36,    37,    38,    39,    40,
      34,    35,    36,    37,    38,    39,    40,   134,   264,    41,
      95,   131,   171,   175,   137,   244,     0,     0,     0,     0,
       0,   137,     0,     0,     0,   185,   186,   187,   137,     0,
       0,     0,    42,   188,   189,   190,     0,     0,   191,    42,
      68,   192,     0,    43,     0,     0,     0,     0,     1,    81,
      43,     0,    82,    34,    35,    36,    37,    38,    39,    40,
      43,     0,     0,     0,     0,   149,     0,     0,     0,     0,
       0,     0,     0,     0,   150,     0,     0,     0,     0,     0,
      42,     2,     3,     0,     0,     0,     0,   270,     0,     0,
       4,    43,     0,     5,   272,     0,     6,   193,    43,     7,
     249,     0,     0,     8,     9,    43,     0,     0,     0,     0,
     194,     0,   195,   196,   197,   198,   199,   200,    10,     0,
       0,     0,     0,    11,     0,     0,     0,     0,     0,     0,
       0,     0,   151,   152,     0,   153,   154,   155,   250,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43
};

static const yytype_int16 yycheck[] =
{
       4,   113,    27,     3,     3,   132,   110,    39,    39,    53,
     110,     3,     4,     5,     6,     7,     8,     9,    92,    55,
      27,    57,    52,    97,    54,    29,    30,    31,    32,    33,
       3,     4,     5,     6,     7,     8,     9,    41,    42,    42,
      52,   145,    54,    47,    20,    49,    50,    91,   148,    45,
      54,    93,    58,     3,     4,     5,     6,     7,     8,     9,
      64,    86,    38,    88,    89,    90,    58,     0,    98,   101,
     101,    75,   145,    98,    93,    98,    99,    81,   151,    86,
      99,    88,    89,    90,    74,    89,    90,    95,    80,    81,
      82,    97,   204,    98,    99,    99,    46,    97,    97,     3,
      97,   136,   108,   138,   101,    97,     3,   211,   212,    59,
      75,    76,    65,   117,   118,   119,   108,    66,   122,    93,
     124,    98,   249,   250,    97,    98,    98,   100,   132,    98,
      99,   135,     3,   137,   138,   108,   209,    39,   211,   212,
     252,   176,   254,   270,   108,   272,     3,    97,   183,   184,
     100,     3,     4,     5,     6,     7,     8,     9,   108,    94,
      39,    99,   166,   167,    39,    99,   170,    98,    20,    99,
     102,     3,     4,     5,     6,     7,     8,     9,    99,    31,
      49,   185,   186,   187,   188,    98,    79,   191,    20,    79,
     194,   195,   196,   197,   198,   199,   200,    79,    21,    31,
       3,     4,     5,     6,     7,     8,     9,    94,    27,   213,
     214,    21,   216,     3,     4,     5,     6,     7,     8,     9,
       3,     4,     5,     6,     7,     8,     9,     3,   232,     3,
      20,     3,   236,    98,    53,    85,    85,    69,    85,    48,
      39,    31,    97,    63,    28,    97,    98,    39,    31,    62,
      71,    72,    18,    74,    83,    58,   108,   261,   262,   263,
      68,    27,     3,    84,    52,    97,   270,    86,   272,    88,
      89,    90,    91,    54,   278,   279,   108,    80,     3,     4,
       5,     6,     7,     8,     9,     3,     4,     5,     6,     7,
       8,     9,    38,     3,    97,     3,     4,     5,     6,     7,
       8,     9,     3,    48,    34,   108,    11,    97,   252,   253,
     254,   255,    98,    98,    97,    98,    11,   117,   108,    85,
      86,    80,    88,    89,    90,   108,     3,     4,     5,     6,
       7,     8,     9,     3,     4,     5,     6,     7,     8,     9,
       3,     4,     5,     6,     7,     8,     9,   102,   232,    74,
      64,    99,   127,   133,    31,   201,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    15,    16,    17,    31,    -1,
      -1,    -1,    97,    23,    24,    25,    -1,    -1,    28,    97,
      98,    31,    -1,   108,    -1,    -1,    -1,    -1,    22,    97,
     108,    -1,   100,     3,     4,     5,     6,     7,     8,     9,
     108,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      97,    55,    56,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      64,   108,    -1,    67,    97,    -1,    70,    87,   108,    73,
      53,    -1,    -1,    77,    78,   108,    -1,    -1,    -1,    -1,
     100,    -1,   102,   103,   104,   105,   106,   107,    92,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    86,    -1,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    22,    55,    56,    64,    67,    70,    73,    77,    78,
      92,    97,   110,   111,   112,   113,   114,   115,   116,   117,
     144,   147,   151,   152,   155,   156,   157,   158,   159,    71,
      72,    74,    84,    45,     3,     4,     5,     6,     7,     8,
       9,    74,    97,   108,   139,   140,   141,    42,   153,    93,
      74,    46,    59,   118,   119,   114,   115,     0,    95,   139,
     139,   139,   139,   139,    97,   101,     3,   139,    98,   139,
     142,   143,     3,    65,   139,    66,   154,   139,   139,    93,
     122,    97,   100,   120,   121,   139,    98,    98,   112,    55,
      57,   145,   146,    98,   100,   142,     3,    39,    98,    99,
     108,     3,   148,   149,   138,   139,    94,   129,    39,     3,
      97,   123,   124,   125,   128,   129,   139,    99,    39,    58,
      80,    81,    82,   139,    58,    80,   139,    99,    98,    98,
     112,   143,   102,    99,   122,    99,    20,    31,    97,   136,
     137,   139,   112,    39,   101,    97,   124,   128,    99,    18,
      27,    85,    86,    88,    89,    90,   126,   126,    49,   130,
     131,   133,    98,   121,   139,   139,    79,    79,   139,   139,
      79,   146,    21,   137,   139,   149,    94,   150,   139,   136,
     139,   136,   139,    52,    54,    15,    16,    17,    23,    24,
      25,    28,    31,    87,   100,   102,   103,   104,   105,   106,
     107,    21,     3,     3,   125,   128,    98,    98,   124,    85,
     125,    85,    85,    48,    63,   134,    39,   139,   139,   139,
      75,    76,   160,    69,   136,    98,   136,   136,   139,   139,
     139,   139,    97,    20,    38,   139,    28,   139,   139,   139,
     139,   139,   139,   139,   160,    39,   101,    39,   125,    53,
      91,   127,   125,   128,   125,   128,   139,   139,    62,   132,
     139,    83,    68,    52,   138,    38,   139,     3,     3,     3,
      97,   137,    97,   137,   127,   127,   127,   127,    48,    34,
     135,   139,   139,   139,    98,   137,   137,   139,   139,    98,
      98
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, yyscanner, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, yyscanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, yyscanner); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Trans_yyscan_t yyscanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Trans_yyscan_t yyscanner;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (yyscanner);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Trans_yyscan_t yyscanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Trans_yyscan_t yyscanner;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, Trans_yyscan_t yyscanner)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, yyscanner)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    Trans_yyscan_t yyscanner;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , yyscanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, yyscanner); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, Trans_yyscan_t yyscanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, yyscanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    Trans_yyscan_t yyscanner;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (yyscanner);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (Trans_yyscan_t yyscanner);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (Trans_yyscan_t yyscanner)
#else
int
yyparse (yyscanner)
    Trans_yyscan_t yyscanner;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 121 "grammar.y"
    {
														TransYY_yyget_extra(yyscanner)->parsetree_ = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 126 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (3)]._astNode_);
													}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 130 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 135 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 138 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 141 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 144 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_); 
													}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 147 "grammar.y"
    {	
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 150 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 153 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 156 "grammar.y"
    {
														Operator* oper = new Operator ("BEGIN");
														(yyval._astNode_) = oper; 
													}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 160 "grammar.y"
    {
														Operator* oper = new Operator ("COMMIT");
														(yyval._astNode_) = oper;
													}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 164 "grammar.y"
    {
														Operator* oper = new Operator ("ROLLBACK");	
														(yyval._astNode_) = oper; 	
													}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 168 "grammar.y"
    { (yyval._astNode_) = NULL;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 177 "grammar.y"
    { (yyval._astNode_) = (yyvsp[(2) - (3)]._astNode_); }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 178 "grammar.y"
    { (yyval._astNode_) = (yyvsp[(2) - (3)]._astNode_); }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 183 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_);
													}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 188 "grammar.y"
    { (yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_); }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 192 "grammar.y"
    {
														SelectStmt* select = new SelectStmt ();
														select->setTargetList ((yyvsp[(2) - (5)]._targetList_));
														select->setFromStmt((yyvsp[(3) - (5)]._fromStmt_));
														select->setWhereStmt((yyvsp[(4) - (5)]._whereStmt_));
														/*	
														AggregateStmt* agg = new AggregateStmt ();
														agg->setGroupByStmt ((GroupByStmt*)$5);		
														agg->setHavingStmt((HavingStmt*)$6);
														agg->setOrderByStmt((OrderByStmt*)$7) ;
														*/	
														select->setAggStmt ((yyvsp[(5) - (5)]._aggrStmt_));
														(yyval._astNode_) = select; 
													}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 207 "grammar.y"
    {(yyval._targetList_) = (yyvsp[(2) - (2)]._targetList_);}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 208 "grammar.y"
    {(yyval._targetList_) = (TargetList*)NULL;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 211 "grammar.y"
    { (yyval._astNode_) = NULL;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 212 "grammar.y"
    { (yyval._astNode_) = NULL;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 213 "grammar.y"
    { (yyval._astNode_) = NULL;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 216 "grammar.y"
    {
                                                        TargetList* targetList = new TargetList ();
                                                        targetList->addEntry ((yyvsp[(1) - (1)]._targetEntry_)) ;
                                                        (yyval._targetList_) = targetList;
                                                    }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 222 "grammar.y"
    {
														TargetList* target(NULL); 
														if ((yyvsp[(1) - (3)]._targetList_)) {
															target = (yyvsp[(1) - (3)]._targetList_); 
															target->mergeEntries ((yyvsp[(3) - (3)]._targetEntry_)) ;
														} else {
															target = new TargetList () ;
															target->addEntry ((yyvsp[(3) - (3)]._targetEntry_)) ;
														}
														(yyval._targetList_) = target; 
													}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 235 "grammar.y"
    {
														TargetEntry* targetEntry = new TargetEntry ((yyvsp[(1) - (1)]._astNode_), NULL);
														(yyval._targetEntry_) = targetEntry;	
													}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 239 "grammar.y"
    {
														TargetEntry* targetEntry = new TargetEntry ((yyvsp[(2) - (3)]._astNode_), NULL) ;	
														(yyval._targetEntry_) = targetEntry;
													}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 243 "grammar.y"
    {
														TargetEntry* targetEntry = new TargetEntry ((yyvsp[(2) - (5)]._astNode_), (yyvsp[(5) - (5)]._astNode_)) ;	
														(yyval._targetEntry_) = targetEntry;
													}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 247 "grammar.y"
    {
														ASTNode* node = new ASTNode ("*") ;
														TargetEntry* targetEntry = new TargetEntry (node, NULL);
														(yyval._targetEntry_) = targetEntry;	
													}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 252 "grammar.y"
    {
														TargetEntry* targetEntry = new TargetEntry ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_));
														(yyval._targetEntry_) = targetEntry;	

													}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 259 "grammar.y"
    {
														FromStmt* from = new FromStmt(); 
														from->setTableEntryList ((yyvsp[(2) - (2)]._tableList_));	
														(yyval._fromStmt_) = from; 	
													}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 264 "grammar.y"
    {
														(yyval._fromStmt_) = NULL ; 
													}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 270 "grammar.y"
    {
														TableEntryList* tableList = NULL;
														if ((yyvsp[(1) - (3)]._tableList_)) {	
															tableList = (yyvsp[(1) - (3)]._tableList_); 
														} else  
															tableList = new TableEntryList ();

														tableList->addTableEntry((yyvsp[(3) - (3)]._tableEntry_)) ;
														(yyval._tableList_) = tableList; 		
													}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 280 "grammar.y"
    {
														TableEntryList* tableList = new TableEntryList ();
														tableList->addTableEntry ((yyvsp[(1) - (1)]._tableEntry_));
														(yyval._tableList_) = tableList; 
													}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 285 "grammar.y"
    {
														TableEntryList* tableList = new TableEntryList ();
														tableList->addTableEntry ((yyvsp[(2) - (3)]._tableEntry_)) ;
														(yyval._tableList_) = tableList;
													}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 292 "grammar.y"
    {
														(yyval._tableEntry_) = (yyvsp[(1) - (1)]._tableEntry_); 
													}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 295 "grammar.y"
    {//for joined_table
														(yyval._tableEntry_) = (yyvsp[(1) - (1)]._tableEntry_);
													}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 298 "grammar.y"
    {
														(yyval._tableEntry_) = (yyvsp[(2) - (3)]._tableEntry_); 
													}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 301 "grammar.y"
    {//for joined_table with alias
														ASTNode* aliasName = new ASTNode ((yyvsp[(5) - (5)]._str_)) ;	
														JoinedTableEntry* joinedTablePtr = NULL; 
														if ((yyvsp[(2) - (5)]._tableEntry_)) {
															joinedTablePtr = (JoinedTableEntry*) (yyvsp[(2) - (5)]._tableEntry_);
															joinedTablePtr->setAliasName (aliasName);
														}
														free((yyvsp[(5) - (5)]._str_));
														(yyval._tableEntry_) = joinedTablePtr ;
													}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 313 "grammar.y"
    {
														ASTNode* tableName = new ASTNode ((yyvsp[(1) - (1)]._str_));
														TableEntry* table = new TableEntry (tableName, NULL) ;
														free ((yyvsp[(1) - (1)]._str_)) ;
														(yyval._tableEntry_) = table; //to improve the memory allocation/free efficiency.
													}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 319 "grammar.y"
    {	//for database.table mode.
														char tbBuffer[128] = {0} ;
														sprintf (tbBuffer,"%s.%s", (yyvsp[(1) - (3)]._str_), (yyvsp[(3) - (3)]._str_));//format: database.table
														ASTNode* tableName = new ASTNode(tbBuffer);
															
														TableEntry* table = new TableEntry (tableName, NULL);
														free ((yyvsp[(1) - (3)]._str_)); 
														free ((yyvsp[(3) - (3)]._str_));	
														(yyval._tableEntry_) = table;
													}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 329 "grammar.y"
    { //for schema.database.table mode.
                                                        char tbBuffer[128] = {0} ;
                                                        sprintf (tbBuffer,"%s.%s.%s", (yyvsp[(1) - (5)]._str_), (yyvsp[(3) - (5)]._str_),(yyvsp[(5) - (5)]._str_));//format: database.table
                                                        ASTNode* tableName = new ASTNode(tbBuffer);
   
                                                        TableEntry* table = new TableEntry (tableName, NULL);
                                                        free ((yyvsp[(1) - (5)]._str_));
                                                        free ((yyvsp[(3) - (5)]._str_));
														free ((yyvsp[(5) - (5)]._str_));
                                                        (yyval._tableEntry_) = table;
													}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 340 "grammar.y"
    {
														ASTNode* tableName = new ASTNode ((yyvsp[(1) - (3)]._str_));
														free ((yyvsp[(1) - (3)]._str_)) ;
														ASTNode* alias = new ASTNode ((yyvsp[(3) - (3)]._str_));
														free ((yyvsp[(3) - (3)]._str_)) ;
														TableEntry* table = new TableEntry (tableName, alias) ;
														(yyval._tableEntry_) = table; //to improve the memory allocation/free efficiency.
											
													}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 349 "grammar.y"
    {
														char tbBuffer[128] = {0} ;
														sprintf (tbBuffer,"%s.%s", (yyvsp[(1) - (5)]._str_), (yyvsp[(3) - (5)]._str_));//format: database.table
														ASTNode* tableName = new ASTNode(tbBuffer);
														
														ASTNode* alias = new ASTNode ((yyvsp[(5) - (5)]._str_));	
																
														TableEntry* table = new TableEntry (tableName, alias);
														free ((yyvsp[(1) - (5)]._str_)); 
														free ((yyvsp[(3) - (5)]._str_));	
														free ((yyvsp[(5) - (5)]._str_));
														(yyval._tableEntry_) = table;
													}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 363 "grammar.y"
    {
														(yyval._join_type_) = JOINED_TABLE_LEFT_JOIN;
													}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 366 "grammar.y"
    {
														(yyval._join_type_) = JOINED_TABLE_RIGHT_JOIN ;
													}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 369 "grammar.y"
    {
														(yyval._join_type_) = JOINED_TABLE_FULL_JOIN; 
													}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 372 "grammar.y"
    {
														(yyval._join_type_) = JOINED_TABLE_INNER_JOIN;
													}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 375 "grammar.y"
    {
														(yyval._join_type_) = JOINED_TABLE_NATURE_JOIN;
													}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 380 "grammar.y"
    {
														JoinedQual* qual = new JoinedQual ((yyvsp[(2) - (2)]._expr_), JOINEDQUAL_ON);
														(yyval._astNode_) = qual; 
													}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 384 "grammar.y"
    {
														JoinedQual* qual = new JoinedQual ((yyvsp[(3) - (4)]._expr_), JOINEDQUAL_ON);
														(yyval._astNode_) = qual; 
													}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 388 "grammar.y"
    {
														JoinedQual* qual = new JoinedQual ((yyvsp[(2) - (2)]._expr_), JOINEDQUAL_USING);
														(yyval._astNode_) = qual; 
													}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 392 "grammar.y"
    {
														JoinedQual* qual = new JoinedQual ((yyvsp[(3) - (4)]._expr_), JOINEDQUAL_USING);
														(yyval._astNode_) = qual; 
													}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 398 "grammar.y"
    {
														JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ((yyvsp[(1) - (4)]._tableEntry_),(yyvsp[(4) - (4)]._tableEntry_),NULL, NULL) ;
														joinedTablePtr->setJoinedType (JOINED_TABLE_CROSS_JOIN);
														(yyval._tableEntry_) = joinedTablePtr; 
							   					  }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 404 "grammar.y"
    {
														JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ((yyvsp[(1) - (5)]._tableEntry_), (yyvsp[(4) - (5)]._tableEntry_), NULL, (yyvsp[(5) - (5)]._astNode_));
														joinedTablePtr->setJoinedType ((yyvsp[(2) - (5)]._join_type_)) ;
														(yyval._tableEntry_) = joinedTablePtr ;
												  }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 410 "grammar.y"
    {
														JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ((yyvsp[(1) - (4)]._tableEntry_), (yyvsp[(3) - (4)]._tableEntry_), NULL, (yyvsp[(4) - (4)]._astNode_));
														joinedTablePtr->setJoinedType (JOINED_TABLE_INNER_JOIN);
														(yyval._tableEntry_) = joinedTablePtr; 	
												 }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 416 "grammar.y"
    {
                                                        JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ((yyvsp[(1) - (5)]._tableEntry_), (yyvsp[(4) - (5)]._tableEntry_), NULL, (yyvsp[(5) - (5)]._astNode_));
                                                        joinedTablePtr->setJoinedType ((yyvsp[(2) - (5)]._join_type_)) ;
                                                        (yyval._tableEntry_) = joinedTablePtr ;
												}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 422 "grammar.y"
    {
                                                        JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ((yyvsp[(1) - (5)]._tableEntry_), (yyvsp[(4) - (5)]._tableEntry_), NULL, (yyvsp[(5) - (5)]._astNode_));
                                                        joinedTablePtr->setJoinedType ((yyvsp[(2) - (5)]._join_type_)) ;
                                                        (yyval._tableEntry_) = joinedTablePtr ;
												}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 428 "grammar.y"
    {
                                                        JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ((yyvsp[(1) - (5)]._tableEntry_), (yyvsp[(4) - (5)]._tableEntry_), NULL, (yyvsp[(5) - (5)]._astNode_));
                                                        joinedTablePtr->setJoinedType ((yyvsp[(2) - (5)]._join_type_)) ;
                                                        (yyval._tableEntry_) = joinedTablePtr ;
												}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 436 "grammar.y"
    {
														WhereStmt* where = new WhereStmt (); 
														where->setExpr ((yyvsp[(2) - (2)]._expr_)) ;
														(yyval._whereStmt_) = where; 				
													}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 441 "grammar.y"
    { (yyval._whereStmt_) = NULL ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 445 "grammar.y"
    {
														(yyval._aggrStmt_) = (yyvsp[(1) - (1)]._aggrStmt_); 
													}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 452 "grammar.y"
    {
														if ((yyvsp[(1) - (4)]._astNode_) || (yyvsp[(2) - (4)]._astNode_) || (yyvsp[(3) - (4)]._astNode_) || (yyvsp[(4) - (4)]._astNode_)) {
															AggregateStmt* agg = new AggregateStmt();
															agg->setOrderByStmt((OrderByStmt*)(yyvsp[(3) - (4)]._astNode_));
															agg->setGroupByStmt((GroupByStmt*)(yyvsp[(1) - (4)]._astNode_)) ;
															agg->setHavingStmt((HavingStmt*)(yyvsp[(2) - (4)]._astNode_)) ;
															agg->setLimitStmt ((LimitStmt*) (yyvsp[(4) - (4)]._astNode_)) ;
															(yyval._aggrStmt_) = agg;
														} else
															(yyval._aggrStmt_) = NULL ; 
													}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 465 "grammar.y"
    {
														OrderByStmt* order = new OrderByStmt((yyvsp[(3) - (3)]._astNode_));
														(yyval._astNode_) = order;
													}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 469 "grammar.y"
    {	
														(yyval._astNode_) = NULL; 
													}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 474 "grammar.y"
    {
														GroupByStmt* group = new GroupByStmt ((yyvsp[(3) - (3)]._astNode_));
														(yyval._astNode_) = group;
													}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 478 "grammar.y"
    {	
														(yyval._astNode_) = NULL;
													}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 483 "grammar.y"
    {
														HavingStmt* having = new HavingStmt ((yyvsp[(2) - (2)]._astNode_));
														(yyval._astNode_) = having;	
													}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 487 "grammar.y"
    {
														(yyval._astNode_) = NULL;
													}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 492 "grammar.y"
    {
														LimitStmt* limit = new LimitStmt((yyvsp[(2) - (2)]._astNode_));
														(yyval._astNode_) = limit; 
													}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 496 "grammar.y"
    { 
														(yyval._astNode_) = NULL; 
													}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 503 "grammar.y"
    {
														Operator* oper = new Operator ("AND") ;
														LogicalExpr* expr = new LogicalExpr ((yyvsp[(1) - (3)]._expr_), (yyvsp[(3) - (3)]._expr_), oper);
														(yyval._expr_)= expr;
													}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 509 "grammar.y"
    {
														Operator* oper = new Operator ("OR");
														LogicalExpr* expr = new LogicalExpr ((yyvsp[(1) - (3)]._expr_), (yyvsp[(3) - (3)]._expr_), oper);
														(yyval._expr_) =expr;
													}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 514 "grammar.y"
    {
														Operator* oper = new Operator ("NOT");
														LogicalExpr* expr = new LogicalExpr (NULL, (yyvsp[(2) - (2)]._expr_), oper);
														(yyval._expr_) = expr;
													}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 519 "grammar.y"
    {
														(yyval._expr_) = (yyvsp[(2) - (3)]._expr_);
													}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 525 "grammar.y"
    {
														Operator* oper = new Operator ("=");
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper) ;	
														(yyval._expr_) = arith; 
													}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 530 "grammar.y"
    {
														Operator* oper = new Operator ("+") ;
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper) ;
														(yyval._expr_) = arith;
													}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 535 "grammar.y"
    {
														Operator* oper = new Operator ("-") ;
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper);
														(yyval._expr_) = arith;
													}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 540 "grammar.y"
    {
														Operator* oper = new Operator ("*") ;
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper);
														(yyval._expr_) =arith;
													}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 545 "grammar.y"
    {
														Operator* oper = new Operator ("/") ;
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper);
														(yyval._expr_) =arith;
													}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 550 "grammar.y"
    {
														Operator* oper = new Operator ("<");
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper) ;
														(yyval._expr_) =arith; 
													}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 555 "grammar.y"
    {
														Operator* oper = new Operator (">") ; 
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper);
														(yyval._expr_) = arith;
													}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 560 "grammar.y"
    {
														Operator* oper = new Operator ("<=") ;
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper) ;		
														(yyval._expr_) =arith;
													}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 565 "grammar.y"
    {
														Operator* oper = new Operator (">=") ;
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper) ;		
														(yyval._expr_) =arith;
													}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 570 "grammar.y"
    {
														Operator* oper = new Operator ("!=");
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper);
														(yyval._expr_) = arith;
													}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 582 "grammar.y"
    {
														Operator* oper = new Operator ("!") ;
														ArithExpr* arith = new ArithExpr (NULL, (yyvsp[(2) - (2)]._astNode_), oper);
														(yyval._expr_) =arith;
													}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 587 "grammar.y"
    {
														Operator* oper = new Operator ("LIKE") ;
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (3)]._astNode_), (yyvsp[(3) - (3)]._astNode_), oper);
														(yyval._expr_) =arith;
													}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 592 "grammar.y"
    {	
														Operator* oper = new Operator ("NOT LIKE");
														ArithExpr* arith = new ArithExpr ((yyvsp[(1) - (4)]._astNode_), (yyvsp[(4) - (4)]._astNode_), oper) ;
														(yyval._expr_) = arith; 
													}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 597 "grammar.y"
    {
														BetweenAnd* oper = new BetweenAnd ((yyvsp[(1) - (5)]._astNode_), "BETWEEN");
														ArithExpr* arith = new ArithExpr ((yyvsp[(3) - (5)]._astNode_),(yyvsp[(5) - (5)]._astNode_), oper);
														(yyval._expr_) = arith;
													}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 602 "grammar.y"
    {
														InOper* oper = new InOper ((yyvsp[(1) - (5)]._astNode_), (yyvsp[(4) - (5)]._A_exprList_)) ;
														ArithExpr* arith = new ArithExpr (oper, NULL, NULL);
														(yyval._expr_) =arith;
													}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 607 "grammar.y"
    {
														IsNullOper* oper = new IsNullOper ((yyvsp[(1) - (3)]._astNode_), NULL);
														ArithExpr* arith = new ArithExpr (oper, NULL, NULL);
														(yyval._expr_) = arith ; 
													}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 612 "grammar.y"
    {
														IsNotNullOper* oper  = new IsNotNullOper ((yyvsp[(1) - (4)]._astNode_), NULL);
														ArithExpr* arith = new ArithExpr (oper, NULL, NULL);
														(yyval._expr_) = arith;
													}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 617 "grammar.y"
    {
                                                        IsNotNullOper* oper  = new IsNotNullOper ((yyvsp[(1) - (2)]._astNode_), NULL);
                                                        ArithExpr* arith = new ArithExpr (oper, NULL, NULL);
                                                        (yyval._expr_) = arith;
													}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 625 "grammar.y"
    {
														A_ExprList* aexprs (NULL);
														if ((yyvsp[(1) - (3)]._A_exprList_)) 
															aexprs = (A_ExprList*) (yyvsp[(1) - (3)]._A_exprList_); 
														else 
															aexprs = new A_ExprList ();		
														
														aexprs->addExpr((yyvsp[(3) - (3)]._astNode_));
														(yyval._A_exprList_) = aexprs;
													}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 635 "grammar.y"
    {
														A_ExprList* aexprs = new A_ExprList();
														aexprs->addExpr ((yyvsp[(1) - (1)]._astNode_)) ;
														(yyval._A_exprList_) = aexprs;
													}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 643 "grammar.y"
    {
														ASTNode* node = new ASTNode ((yyvsp[(1) - (1)]._str_)) ;
														free ((yyvsp[(1) - (1)]._str_));
														(yyval._astNode_) = node;	
													}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 648 "grammar.y"
    {
														char buffer[257]={0};
														sprintf (buffer, "%s.%s", (yyvsp[(1) - (3)]._str_), (yyvsp[(3) - (3)]._str_));
														ASTNode* node = new ASTNode (buffer);	
														free((yyvsp[(1) - (3)]._str_)); 
														free((yyvsp[(3) - (3)]._str_));
														(yyval._astNode_) = node;
													}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 656 "grammar.y"
    {//integer const.
														ConstValue* node = new ConstValue ((yyvsp[(1) - (1)]._ival_));
														(yyval._astNode_) = node ;
													}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 660 "grammar.y"
    {//float const.
														ConstValue* node = new ConstValue ((yyvsp[(1) - (1)]._str_)) ;
														free ((yyvsp[(1) - (1)]._str_));
														(yyval._astNode_) = node;	
													}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 665 "grammar.y"
    {//string const
														ConstValue* node = new ConstValue ((yyvsp[(1) - (1)]._str_));
														free ((yyvsp[(1) - (1)]._str_)) ; //this value malloc in LexHelper::litbufdup
														(yyval._astNode_) = node; 		
													}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 670 "grammar.y"
    {//bit string const
														ConstValue* node = new ConstValue ((yyvsp[(1) - (1)]._str_));
														free((yyvsp[(1) - (1)]._str_));
														(yyval._astNode_) = node;	
													}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 675 "grammar.y"
    {//bit const 
														ConstValue* node = new ConstValue ();
														(yyval._astNode_) = node;
													}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 679 "grammar.y"
    {
														ConstValue* node = new ConstValue ((yyvsp[(2) - (3)]._str_));
														free ((yyvsp[(2) - (3)]._str_)) ;
														(yyval._astNode_) = node;
													}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 684 "grammar.y"
    {//like @@version_comment
														char buffer[257]={0};
														sprintf (buffer, "%s%s", (yyvsp[(1) - (2)]._str_), (yyvsp[(2) - (2)]._str_));
														ASTNode* node = new ASTNode (buffer);	
														free((yyvsp[(1) - (2)]._str_)); 
														free((yyvsp[(2) - (2)]._str_));
														(yyval._astNode_) = node;    
													}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 692 "grammar.y"
    {
														(yyval._astNode_) = (yyvsp[(1) - (1)]._function_); 
													}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 697 "grammar.y"
    {
														(yyval._str_) = (yyvsp[(1) - (1)]._str_);
													}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 702 "grammar.y"
    {
														ASTNode* funcName = new ASTNode((yyvsp[(1) - (3)]._str_)) ;
														Function* func = new Function(funcName, NULL);
														free((yyvsp[(1) - (3)]._str_)) ;
														(yyval._function_) = func; 
													}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 708 "grammar.y"
    {
														ASTNode* funcName = new ASTNode((yyvsp[(1) - (4)]._str_)) ;
														Function* func = new Function (funcName, (yyvsp[(3) - (4)]._funcArgList_));
														free((yyvsp[(1) - (4)]._str_));
														(yyval._function_) = func; 
													}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 714 "grammar.y"
    {
														ASTNode* funcName = new ASTNode((yyvsp[(1) - (4)]._str_));
														ASTNode* funcarg = new ASTNode ("?") ;
														FunctionArgList* args = new FunctionArgList(funcarg) ;
															
														Function* func = new Function (funcName, args);
														free((yyvsp[(1) - (4)]._str_));
														(yyval._function_) = func; 
													}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 724 "grammar.y"
    {//anonyous function
														ASTNode* funcName = new ASTNode("");
														Function* func = new Function (funcName, NULL) ;
														(yyval._function_) = func; 	
													}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 730 "grammar.y"
    {
														ASTNode* funcName = new ASTNode("") ;
														Function* func = new Function (funcName, (yyvsp[(2) - (3)]._funcArgList_)) ;
														(yyval._function_) = func; 
													}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 738 "grammar.y"
    {
														FunctionArgList* args (NULL);
														if ((yyvsp[(1) - (3)]._funcArgList_))
															args = (FunctionArgList*) (yyvsp[(1) - (3)]._funcArgList_); 
														else 	
															args = new FunctionArgList () ; 
													
														args->addArg ((yyvsp[(3) - (3)]._astNode_)) ;
														(yyval._funcArgList_) = args;
													}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 748 "grammar.y"
    {
														FunctionArgList* args = new FunctionArgList((yyvsp[(1) - (1)]._astNode_)) ;
														(yyval._funcArgList_) = args; 
													}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 755 "grammar.y"
    { (yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_); }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 759 "grammar.y"
    {
														AlterStmt* alter = new AlterStmt (AlterStmt::OBJ_TABLE, (yyvsp[(3) - (4)]._astNode_), (yyvsp[(4) - (4)]._alterOptionList_));
														(yyval._astNode_) = alter;
													}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 765 "grammar.y"
    {
														AlterOptList* optList = (yyvsp[(1) - (3)]._alterOptionList_); 
														optList->addOpt ((yyvsp[(3) - (3)]._alterOpt_));
														(yyval._alterOptionList_) = optList;
													}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 770 "grammar.y"
    {
														AlterOptList* optList = new AlterOptList();
														optList->addOpt ((yyvsp[(1) - (1)]._alterOpt_)); 
														(yyval._alterOptionList_) = optList;
													}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 777 "grammar.y"
    {
														AlterOpt* opt = new AlterOpt (AlterOpt::ACT_ADD, AlterOpt::OPT_EVERY_TYPE, (yyvsp[(2) - (2)]._astNode_));
														(yyval._alterOpt_) = opt;
													}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 781 "grammar.y"
    {
														AlterOpt* opt = new AlterOpt (AlterOpt::ACT_ADD, AlterOpt::OPT_COLUMN, (yyvsp[(3) - (3)]._astNode_));
														(yyval._alterOpt_) = opt;
													}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 785 "grammar.y"
    {
                                                        AlterOpt* opt = new AlterOpt (AlterOpt::ACT_ADD, AlterOpt::OPT_FOREIGN_KEY, (yyvsp[(4) - (6)]._astNode_));
                                                        (yyval._alterOpt_) = opt;															
													}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 789 "grammar.y"
    {
														AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_EVERY_TYPE, (yyvsp[(2) - (2)]._astNode_));
														(yyval._alterOpt_) = opt; 
													}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 793 "grammar.y"
    {
														AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_COLUMN, (yyvsp[(3) - (3)]._astNode_));	
														(yyval._alterOpt_) = opt; 
													}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 797 "grammar.y"
    {
                                                        AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_FOREIGN_KEY, (yyvsp[(4) - (4)]._astNode_));
                                                        (yyval._alterOpt_) = opt;															
													}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 801 "grammar.y"
    {
                                                        AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_PRIMARY_KEY,(yyvsp[(4) - (4)]._astNode_));
                                                        (yyval._alterOpt_) = opt;															
													}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 805 "grammar.y"
    {
                                                        AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_CONSTRAINT,(yyvsp[(3) - (3)]._astNode_));
                                                        (yyval._alterOpt_) = opt;															
													}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 814 "grammar.y"
    {
														UpdateStmt* update = new UpdateStmt ();
														update->setOrigTable ((yyvsp[(2) - (6)]._astNode_)); 
														update->setExpr((yyvsp[(4) - (6)]._exprList_));
														update->setFromStmt((yyvsp[(5) - (6)]._fromStmt_));
														update->setWhereStmt ((yyvsp[(6) - (6)]._whereStmt_));
														(yyval._astNode_) = update; 		
													}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 824 "grammar.y"
    {
														ExprList* exprs = new ExprList ();
														exprs->addExpr ((yyvsp[(1) - (1)]._expr_));
														(yyval._exprList_) = exprs; 
													}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 829 "grammar.y"
    {
														ExprList* exprlist (NULL) ;
														if ((yyvsp[(1) - (3)]._exprList_)) 
															exprlist = (ExprList*)(yyvsp[(1) - (3)]._exprList_); 
														 else 
															exprlist  = new ExprList () ;
														
														exprlist->addExpr ((yyvsp[(3) - (3)]._expr_)) ;
														(yyval._exprList_) = exprlist; 
													}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 841 "grammar.y"
    {
														ASTNode* id = new ASTNode((yyvsp[(1) - (3)]._str_));
														Operator* op = new Operator ("=");
														ArithExpr* eq = new ArithExpr (id, (yyvsp[(3) - (3)]._expr_), op);
														free((yyvsp[(1) - (3)]._str_));
														(yyval._expr_) = eq; 
													
													}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 849 "grammar.y"
    {
                                                        ASTNode* id = new ASTNode((yyvsp[(1) - (3)]._str_));
                                                        Operator* op = new Operator ("=");
                                                        ArithExpr* eq = new ArithExpr (id, (yyvsp[(3) - (3)]._astNode_), op);
                                                        free((yyvsp[(1) - (3)]._str_));
                                                        (yyval._expr_) = eq;														
													}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 858 "grammar.y"
    {
                                                        WhereStmt* where = new WhereStmt ();
                                                        where->setExpr ((yyvsp[(2) - (2)]._expr_)) ;
                                                        (yyval._whereStmt_) = where;														
													}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 863 "grammar.y"
    {
                                                        WhereStmt* where = new WhereStmt ();
                                                        where->setExpr ((Expr*)(yyvsp[(4) - (4)]._astNode_)) ;
                                                        (yyval._whereStmt_) = where;															
													}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 868 "grammar.y"
    {
														(yyval._whereStmt_) = NULL;
													}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 873 "grammar.y"
    {
														DeleteStmt* del = new DeleteStmt ();
														del->setOrigTable ((yyvsp[(3) - (4)]._astNode_));
														del->setWhereStmt ((yyvsp[(4) - (4)]._whereStmt_));
														(yyval._astNode_) = del;
													}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 882 "grammar.y"
    {
														InsertStmt* insert = new InsertStmt ();
														insert->setDestCols((yyvsp[(2) - (3)]._astNode_));	
														insert->setValues((yyvsp[(3) - (3)]._A_exprList_));
														(yyval._astNode_) = insert; 
													}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 890 "grammar.y"
    {
														ASTNode* node = (yyvsp[(2) - (2)]._astNode_); 
														node->getNodeType();
														(yyval._astNode_) = (yyvsp[(2) - (2)]._astNode_); 
													}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 897 "grammar.y"
    {
														(yyval._A_exprList_) = (yyvsp[(2) - (2)]._A_exprList_) ;
													}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 903 "grammar.y"
    {
														DropStmt* drop = new DropStmt(DropStmt::ACT_DATABASE, (yyvsp[(3) - (3)]._astNode_));
														(yyval._astNode_) = drop;
													}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 907 "grammar.y"
    {
														DropStmt* drop = new DropStmt(DropStmt::ACT_USER, (yyvsp[(3) - (3)]._astNode_));
														(yyval._astNode_) = drop;
													}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 911 "grammar.y"
    {
														DropStmt* drop = new DropStmt(DropStmt::ACT_ROLE, (yyvsp[(3) - (3)]._astNode_));
														(yyval._astNode_) = drop;
													}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 918 "grammar.y"
    { (yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_); }
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 919 "grammar.y"
    { (yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_); }
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 920 "grammar.y"
    { (yyval._astNode_) = (yyvsp[(1) - (1)]._astNode_); }
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 925 "grammar.y"
    {
														CreateRuleStmt* createRule = new CreateRuleStmt ((yyvsp[(3) - (7)]._astNode_), (yyvsp[(5) - (7)]._astNode_), (yyvsp[(7) - (7)]._astNode_));
														(yyval._astNode_) = createRule; 		
													}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 932 "grammar.y"
    {
														UpdateRuleStmt* updateRule = new UpdateRuleStmt ((yyvsp[(3) - (7)]._astNode_), (yyvsp[(5) - (7)]._astNode_), (yyvsp[(7) - (7)]._astNode_));
														(yyval._astNode_) = updateRule; 
													}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 938 "grammar.y"
    {
														DropRuleStmt* dropRule = new DropRuleStmt ((yyvsp[(3) - (3)]._astNode_)) ;
														(yyval._astNode_) = dropRule;
													}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 944 "grammar.y"
    {
														ConstValue* opt = new ConstValue ((yyvsp[(1) - (1)]._keyword_));
														(yyval._astNode_) = opt; 
													}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 948 "grammar.y"
    {
														ConstValue* opt = new ConstValue ((yyvsp[(1) - (1)]._keyword_)) ;
														(yyval._astNode_) = opt; 
													}
    break;



/* Line 1455 of yacc.c  */
#line 3549 "grammar.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, yyscanner, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, yyscanner, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, yyscanner, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, yyscanner);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, yyscanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, yyscanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, yyscanner);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, yyscanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 953 "grammar.y"

/*
	the error report. 
*/
static void
TransYY_yyerror(YYLTYPE *yylloc, Trans_yyscan_t yyscanner, const char *msg)
{
}
/*
static void
base_yyerror(YYLTYPE *yylloc, Trans_yyscan_t yyscanner, const char *msg)
{
}
*/

