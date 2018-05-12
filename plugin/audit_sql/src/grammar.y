%{
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
%}

%define api.pure
%pure-parser
%expect 1
%locations
%name-prefix="TransYY_yy"
%lex-param		{Trans_yyscan_t yyscanner}
%parse-param 	{Trans_yyscan_t yyscanner}
/*or using  as following, but for explicity we use the first type.
*using lex-param and parse-param explicitly.
*%param {Trans_yyscan_t yyscanner}
*/

%union{
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
}

%type<_ival_> Iconst
%type<_str_> Sconst
%type<_astNode_> 	a_expr /*a_expr_list*/
%type<_astNode_>	select_clause simple_select select_no_parens select_with_parens 
%type<_astNode_>	stmtblock stmtmulti	stmt SelectStmt AlterStmt UpdateStmt DeleteStmt InsertStmt DropStmt SQLRuleStmt 
%type <_targetEntry_> target_entry
%type <_tableEntry_>  tables_ref table_entry joined_table
%type<_astNode_>    target_opt func_arg_expr insert_target_stmt 
%type<_astNode_>	CreateRuleStmt UpdateRuleStmt DropRuleStmt ActionOptStmt OptionStmt AliasStmt
%type<_expr_>		condition_expr compare_expr set_clause
%type<_exprList_>  set_expr_list 
%type<_A_exprList_> a_expr_list insert_values_stmt 
%type<_function_>	func_expr
%type<_funcArgList_> func_arg_list

%type<_aggrStmt_> aggregate_stmt aggregate_expr 
%type<_astNode_> orderby_expr groupby_expr having_expr limit_expr joined_qual 


%type<_alterOpt_>    AlterStmt_Opt
%type<_alterOptionList_> AlterStmt_Opts

%type<_targetList_> target_list_opt target_list_stmt

%type<_tableList_> from_list 
%type<_fromStmt_>  from_stmt 
%type<_whereStmt_> where_stmt where_current_stmt
%type<_join_type_> joined_type;

%token <_str_>	IDENT FCONST SCONST BCONST XCONST Op
%token <_ival_>	ICONST PARAM
%token			TYPECAST DOT_DOT COLON_EQUALS EQUALS_GREATER
%token			LESS_EQUALS GREATER_EQUALS NOT_EQUALS

%token<_keyword_> CROSS NULLS_P NOT WITH BEGIN_P BETWEEN IN_P IS IS_P INNER LIKE ILIKE SIMILAR NOT_LA FIRST_P LAST_P LIMIT  
					TIME ORDINALITY NULLS_LA NULL_P
						AS TEMPORARY TEMP INTO LOCAL UNLOGGED TABLE ALL GLOBAL  BY GROUP_P ABSOLUTE_P ABORT_P AND ON OR DROP ALTER
							ADD_P COLUMN DISTINCT SUM AVG ORDER_P HAVING UPDATE SET VALUES INSERT OF_P CURRENT_P DELETE DATABASE ROLE 
								CREATE RULE IGNORE KILLSESSION COMMIT_P ROLLBACK_P KEY_P FOREIGN PRIMARY CONSTRAINT REFERENCES USER JOIN
									NATURE NOTNULL FULL LEFT RIGHT USING
%token<_keyword_> SELECT FROM WHERE 

%left AND
%left OR

%nonassoc	SET				/* see relation_expr_opt_alias */
%left		UNION EXCEPT
%left		INTERSECT
%right		NOT
%nonassoc	IS ISNULL NOTNULL	/* IS sets precedence for IS NULL, etc */
%nonassoc	'<' '>' '=' LESS_EQUALS GREATER_EQUALS NOT_EQUALS
%nonassoc	BETWEEN IN_P LIKE ILIKE SIMILAR NOT_LA
%%
stmtblock:	stmtmulti
													{
														TransYY_yyget_extra(yyscanner)->parsetree_ = $1;
													}
			;
stmtmulti:	stmtmulti ';' stmt
													{
														$$ = $1;
													}
			| stmt
													{
														$$ = $1;
													}
			;	
stmt: 
			SelectStmt								{
														$$ = $1;
													} 
			| AlterStmt								{
														$$ = $1;
													}
			| UpdateStmt 							{
														$$ = $1;
													}
			| DeleteStmt							{
														$$ = $1; 
													}
			| InsertStmt 							{	
														$$ = $1;
													}
			| DropStmt 								{
														$$ = $1;
													}
			| SQLRuleStmt							{
														$$ = $1;
													}
			| BEGIN_P								{
														Operator* oper = new Operator ("BEGIN");
														$$ = oper; 
													}	
			| COMMIT_P								{
														Operator* oper = new Operator ("COMMIT");
														$$ = oper;
													}
			| ROLLBACK_P								{
														Operator* oper = new Operator ("ROLLBACK");	
														$$ = oper; 	
													}
			|/*empty*/ 								{ $$ = NULL;}
			;

/*The SelectStmt definition*/
SelectStmt: select_no_parens						%prec UMINUS
			| select_with_parens					%prec UMINUS
			;

select_with_parens:
			'(' select_no_parens ')'				{ $$ = $2; }
			| '(' select_with_parens ')'			{ $$ = $2; }
			;

select_no_parens:
			select_clause 
													{
														$$ = $1;
													}
			;
select_clause:
			simple_select							{ $$ = $1; }
			;
simple_select:
			SELECT target_list_opt from_stmt where_stmt  aggregate_stmt /*groupby_expr having_expr orderby_expr*/ 
													{
														SelectStmt* select = new SelectStmt ();
														select->setTargetList ($2);
														select->setFromStmt($3);
														select->setWhereStmt($4);
														/*	
														AggregateStmt* agg = new AggregateStmt ();
														agg->setGroupByStmt ((GroupByStmt*)$5);		
														agg->setHavingStmt((HavingStmt*)$6);
														agg->setOrderByStmt((OrderByStmt*)$7) ;
														*/	
														select->setAggStmt ($5);
														$$ = select; 
													}
			;
target_list_opt: target_opt target_list_stmt		{$$ = $2;}
			|/*empty*/								{$$ = (TargetList*)NULL;}	
			;
target_opt:
			DISTINCT								{ $$ = NULL;}
			| ALL									{ $$ = NULL;}
			| /**/									{ $$ = NULL;}
			;	
target_list_stmt:
            target_entry                           {
                                                        TargetList* targetList = new TargetList ();
                                                        targetList->addEntry ($1) ;
                                                        $$ = targetList;
                                                    }
			|
			target_list_stmt ',' target_entry		{
														TargetList* target(NULL); 
														if ($1) {
															target = $1; 
															target->mergeEntries ($3) ;
														} else {
															target = new TargetList () ;
															target->addEntry ($3) ;
														}
														$$ = target; 
													}	
			;
target_entry:	
			a_expr									{
														TargetEntry* targetEntry = new TargetEntry ($1, NULL);
														$$ = targetEntry;	
													}
			| '(' a_expr ')'						{
														TargetEntry* targetEntry = new TargetEntry ($2, NULL) ;	
														$$ = targetEntry;
													}
			| '(' a_expr ')' AS a_expr				{
														TargetEntry* targetEntry = new TargetEntry ($2, $5) ;	
														$$ = targetEntry;
													}
			|	'*'									{
														ASTNode* node = new ASTNode ("*") ;
														TargetEntry* targetEntry = new TargetEntry (node, NULL);
														$$ = targetEntry;	
													}
			|	a_expr AS a_expr					{
														TargetEntry* targetEntry = new TargetEntry ($1, $3);
														$$ = targetEntry;	

													}
			;
				
from_stmt:	FROM from_list							{
														FromStmt* from = new FromStmt(); 
														from->setTableEntryList ($2);	
														$$ = from; 	
													}
			|/*without from stmt*/					{
														$$ = NULL ; 
													}
			;


from_list:	from_list ',' tables_ref				{
														TableEntryList* tableList = NULL;
														if ($1) {	
															tableList = $1; 
														} else  
															tableList = new TableEntryList ();

														tableList->addTableEntry($3) ;
														$$ = tableList; 		
													}
			| tables_ref							{
														TableEntryList* tableList = new TableEntryList ();
														tableList->addTableEntry ($1);
														$$ = tableList; 
													}
			| '(' tables_ref ')' OptionStmt         {
														TableEntryList* tableList = new TableEntryList ();
														TableEntry* tableEntry = (TableEntry*) $2;
														tableList->setAliasName ($4);	
														tableList->addTableEntry (tableEntry) ;
														$$ = tableList;
													}
			| select_with_parens OptionStmt			{
														TableEntryList* tableList = new TableEntryList ();
														TableEntry* entry = new TableEntry ($1, NULL, TABLETYPES_SUBSELECT);
														tableList->addTableEntry (entry) ;
														tableList->setAliasName ($2);
														$$ = tableList;
													}
			;

tables_ref:	table_entry 							{
														$$ = $1; 
													}
			| joined_table 							{//for joined_table
														$$ = $1;
													}
/*
			| '(' joined_table ')' OptionStmt			{//for joined_table with alias
														//ASTNode* aliasName = new ASTNode ($5) ;	
														JoinedTableEntry* joinedTablePtr = NULL; 
														if ($2) {
															joinedTablePtr = (JoinedTableEntry*) $2;
															joinedTablePtr->setAliasName ($4);
														}
														//free($5);
														$$ = joinedTablePtr ;
													}
*/
			;
				
table_entry : IDENT 								{//We can improve the rules of alias option in future by using OptionStmt, but now we do not. 
														ASTNode* tableName = new ASTNode ($1);
														TableEntry* table = new TableEntry (tableName, NULL) ;
														free ($1) ;
														$$ = table; //to improve the memory allocation/free efficiency.
													}
			| IDENT '.' IDENT						{	//for database.table mode.
														char tbBuffer[128] = {0} ;
														sprintf (tbBuffer,"%s.%s", $1, $3);//format: database.table
														ASTNode* tableName = new ASTNode(tbBuffer);
															
														TableEntry* table = new TableEntry (tableName, NULL);
														free ($1); 
														free ($3);	
														$$ = table;
													}
			| IDENT '.' IDENT '.' IDENT 			{ //for schema.database.table mode.
                                                        char tbBuffer[128] = {0} ;
                                                        sprintf (tbBuffer,"%s.%s.%s", $1, $3,$5);//format: database.table
                                                        ASTNode* tableName = new ASTNode(tbBuffer);
   
                                                        TableEntry* table = new TableEntry (tableName, NULL);
                                                        free ($1);
                                                        free ($3);
														free ($5);
                                                        $$ = table;
													}
			| IDENT AS IDENT						{
														ASTNode* tableName = new ASTNode ($1);
														free ($1) ;
														ASTNode* alias = new ASTNode ($3);
														free ($3) ;
														TableEntry* table = new TableEntry (tableName, alias) ;
														$$ = table; //to improve the memory allocation/free efficiency.
											
													}
			| IDENT '.' IDENT AS IDENT				{
														char tbBuffer[128] = {0} ;
														sprintf (tbBuffer,"%s.%s", $1, $3);//format: database.table
														ASTNode* tableName = new ASTNode(tbBuffer);
														
														ASTNode* alias = new ASTNode ($5);	
																
														TableEntry* table = new TableEntry (tableName, alias);
														free ($1); 
														free ($3);	
														free ($5);
														$$ = table;
													}
			;
joined_type:  LEFT 									{
														$$ = JOINED_TABLE_LEFT_JOIN;
													}
			 | RIGHT 								{
														$$ = JOINED_TABLE_RIGHT_JOIN ;
													}
			 | FULL                                 {
														$$ = JOINED_TABLE_FULL_JOIN; 
													}
			 | INNER                                {
														$$ = JOINED_TABLE_INNER_JOIN;
													}
			| NATURE 								{
														$$ = JOINED_TABLE_NATURE_JOIN;
													} //Here, we can use an empty entry for nature joni type. therefore, in joinedtable rules can simplified.
			;
				
joined_qual:ON compare_expr 						{
														JoinedQual* qual = new JoinedQual ($2, JOINEDQUAL_ON);
														$$ = qual; 
													}
			| ON '(' compare_expr ')'				{
														JoinedQual* qual = new JoinedQual ($3, JOINEDQUAL_ON);
														$$ = qual; 
													}
			| USING compare_expr 					{
														JoinedQual* qual = new JoinedQual ($2, JOINEDQUAL_USING);
														$$ = qual; 
													}
			| USING '(' compare_expr ')'			{
														JoinedQual* qual = new JoinedQual ($3, JOINEDQUAL_USING);
														$$ = qual; 
													}
			;	
joined_table:
			table_entry CROSS JOIN table_entry    {
														JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ($1,$4,NULL, NULL) ;
														joinedTablePtr->setJoinedType (JOINED_TABLE_CROSS_JOIN);
														$$ = joinedTablePtr; 
							   					  } 
			| table_entry joined_type JOIN table_entry joined_qual  
												 {
														JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ($1, $4, NULL, $5);
														joinedTablePtr->setJoinedType ($2) ;
														$$ = joinedTablePtr ;
												  }
			| table_entry JOIN table_entry joined_qual    
												 {
														JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ($1, $3, NULL, $4);
														joinedTablePtr->setJoinedType (JOINED_TABLE_INNER_JOIN);
														$$ = joinedTablePtr; 	
												 }
			| joined_table joined_type JOIN joined_table joined_qual 
												{
                                                        JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ($1, $4, NULL, $5);
                                                        joinedTablePtr->setJoinedType ($2) ;
                                                        $$ = joinedTablePtr ;
												}
			| joined_table joined_type JOIN table_entry joined_qual 
												{
                                                        JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ($1, $4, NULL, $5);
                                                        joinedTablePtr->setJoinedType ($2) ;
                                                        $$ = joinedTablePtr ;
												}
			| table_entry joined_type JOIN joined_table joined_qual	
												{
                                                        JoinedTableEntry* joinedTablePtr = new JoinedTableEntry ($1, $4, NULL, $5);
                                                        joinedTablePtr->setJoinedType ($2) ;
                                                        $$ = joinedTablePtr ;
												}
			;
	
where_stmt : WHERE condition_expr 
													{
														WhereStmt* where = new WhereStmt (); 
														where->setExpr ($2) ;
														$$ = where; 				
													}
			|/*without where stmt*/					{ $$ = NULL ;}
			;

aggregate_stmt:
			aggregate_expr 							{
														$$ = $1; 
													}
			;

aggregate_expr:
			groupby_expr having_expr orderby_expr limit_expr	
													{
														if ($1 || $2 || $3 || $4) {
															AggregateStmt* agg = new AggregateStmt();
															agg->setOrderByStmt((OrderByStmt*)$3);
															agg->setGroupByStmt((GroupByStmt*)$1) ;
															agg->setHavingStmt((HavingStmt*)$2) ;
															agg->setLimitStmt ((LimitStmt*) $4) ;
															$$ = agg;
														} else
															$$ = NULL ; 
													}
			;
orderby_expr:	
			ORDER_P BY a_expr						{
														OrderByStmt* order = new OrderByStmt($3);
														$$ = order;
													}
			| /**/									{	
														$$ = NULL; 
													}
			;
groupby_expr:
			 GROUP_P BY a_expr						{
														GroupByStmt* group = new GroupByStmt ($3);
														$$ = group;
													}
			|/**/									{	
														$$ = NULL;
													}
			;
having_expr:
			HAVING a_expr							{
														HavingStmt* having = new HavingStmt ($2);
														$$ = having;	
													}
			| /**/									{
														$$ = NULL;
													}
			;
limit_expr :
			LIMIT a_expr 							{
														LimitStmt* limit = new LimitStmt($2);
														$$ = limit; 
													}
			| /**/									{ 
														$$ = NULL; 
													}
			;
condition_expr:
			compare_expr 							
			|	condition_expr AND condition_expr 
													{
														Operator* oper = new Operator ("AND") ;
														LogicalExpr* expr = new LogicalExpr ($1, $3, oper);
														$$= expr;
													}
			|	condition_expr OR condition_expr     
													{
														Operator* oper = new Operator ("OR");
														LogicalExpr* expr = new LogicalExpr ($1, $3, oper);
														$$ =expr;
													} 
			|	NOT condition_expr 	%prec	NOT 	{
														Operator* oper = new Operator ("NOT");
														LogicalExpr* expr = new LogicalExpr (NULL, $2, oper);
														$$ = expr;
													}	
			| '(' condition_expr ')'				{
														$$ = $2;
													}
			;

compare_expr:
			a_expr '=' a_expr		%prec RIGHT		{
														Operator* oper = new Operator ("=");
														ArithExpr* arith = new ArithExpr ($1, $3, oper) ;	
														$$ = arith; 
													}
			|	a_expr '+' a_expr 					{
														Operator* oper = new Operator ("+") ;
														ArithExpr* arith = new ArithExpr ($1, $3, oper) ;
														$$ = arith;
													}
			|   a_expr '-' a_expr					{
														Operator* oper = new Operator ("-") ;
														ArithExpr* arith = new ArithExpr ($1, $3, oper);
														$$ = arith;
													}
			|	a_expr '*' a_expr					{
														Operator* oper = new Operator ("*") ;
														ArithExpr* arith = new ArithExpr ($1, $3, oper);
														$$ =arith;
													}
			| 	a_expr '/' a_expr					{
														Operator* oper = new Operator ("/") ;
														ArithExpr* arith = new ArithExpr ($1, $3, oper);
														$$ =arith;
													}
			|	a_expr '<' a_expr 					{
														Operator* oper = new Operator ("<");
														ArithExpr* arith = new ArithExpr ($1, $3, oper) ;
														$$ =arith; 
													}
			|	a_expr '>' a_expr 					{
														Operator* oper = new Operator (">") ; 
														ArithExpr* arith = new ArithExpr ($1, $3, oper);
														$$ = arith;
													}
			|	a_expr LESS_EQUALS a_expr 			{
														Operator* oper = new Operator ("<=") ;
														ArithExpr* arith = new ArithExpr ($1, $3, oper) ;		
														$$ =arith;
													}
			|	a_expr GREATER_EQUALS a_expr 			{
														Operator* oper = new Operator (">=") ;
														ArithExpr* arith = new ArithExpr ($1, $3, oper) ;		
														$$ =arith;
													}
			|	a_expr NOT_EQUALS a_expr			{
														Operator* oper = new Operator ("!=");
														ArithExpr* arith = new ArithExpr ($1, $3, oper);
														$$ = arith;
													}
/*
			|	NOT a_expr 							{
														Operator* oper = new Operator ("!") ;
														ArithExpr* arith = new ArithExpr(NULL, $2, oper);
														$$ =arith;
													}
*/
			|	NOT_LA a_expr %prec NOT				{
														Operator* oper = new Operator ("!") ;
														ArithExpr* arith = new ArithExpr (NULL, $2, oper);
														$$ =arith;
													}
			| 	a_expr LIKE a_expr					{
														Operator* oper = new Operator ("LIKE") ;
														ArithExpr* arith = new ArithExpr ($1, $3, oper);
														$$ =arith;
													}
			| 	a_expr NOT_LA LIKE a_expr 			{	
														Operator* oper = new Operator ("NOT LIKE");
														ArithExpr* arith = new ArithExpr ($1, $4, oper) ;
														$$ = arith; 
													}
			| a_expr BETWEEN a_expr AND a_expr 		{
														BetweenAnd* oper = new BetweenAnd ($1, "BETWEEN");
														ArithExpr* arith = new ArithExpr ($3,$5, oper);
														$$ = arith;
													}
			| a_expr IN_P '(' a_expr_list ')'		{
														InOper* oper = new InOper ($1, $4) ;
														ArithExpr* arith = new ArithExpr (oper, NULL, NULL);
														$$ =arith;
													}
			| a_expr IS NULL_P	%prec IS			{
														IsNullOper* oper = new IsNullOper ($1, NULL);
														ArithExpr* arith = new ArithExpr (oper, NULL, NULL);
														$$ = arith ; 
													}
			| a_expr IS NOT NULL_P %prec IS 		{
														IsNotNullOper* oper  = new IsNotNullOper ($1, NULL);
														ArithExpr* arith = new ArithExpr (oper, NULL, NULL);
														$$ = arith;
													}
			| a_expr NOTNULL 						{
                                                        IsNotNullOper* oper  = new IsNotNullOper ($1, NULL);
                                                        ArithExpr* arith = new ArithExpr (oper, NULL, NULL);
                                                        $$ = arith;
													}
			;

a_expr_list:
			a_expr_list ',' a_expr		%prec LEFT	{
														A_ExprList* aexprs (NULL);
														if ($1) 
															aexprs = (A_ExprList*) $1; 
														else 
															aexprs = new A_ExprList ();		
														
														aexprs->addExpr($3);
														$$ = aexprs;
													}
			|	a_expr 								{
														A_ExprList* aexprs = new A_ExprList();
														aexprs->addExpr ($1) ;
														$$ = aexprs;
													}
			;

a_expr:	
		IDENT 										{
														ASTNode* node = new ASTNode ($1) ;
														free ($1);
														$$ = node;	
													}
		|	IDENT '.' IDENT							{
														char buffer[257]={0};
														sprintf (buffer, "%s.%s", $1, $3);
														ASTNode* node = new ASTNode (buffer);	
														free($1); 
														free($3);
														$$ = node;
													}
		| Iconst									{//integer const.
														ConstValue* node = new ConstValue ($1);
														$$ = node ;
													}
		| FCONST 									{//float const.
														ConstValue* node = new ConstValue ($1) ;
														free ($1);
														$$ = node;	
													}
		| Sconst									{//string const
														ConstValue* node = new ConstValue ($1);
														free ($1) ; //this value malloc in LexHelper::litbufdup
														$$ = node; 		
													}
		| BCONST									{//bit string const
														ConstValue* node = new ConstValue ($1);
														free($1);
														$$ = node;	
													}
		| XCONST									{//bit const 
														ConstValue* node = new ConstValue ($1);
														$$ = node;
													}
		|'"' IDENT '"'								{
														ConstValue* node = new ConstValue ($2);
														free ($2) ;
														$$ = node;
													}
		| Op IDENT									{//like @@version_comment
														char buffer[257]={0};
														sprintf (buffer, "%s%s", $1, $2);
														ASTNode* node = new ASTNode (buffer);	
														free($1); 
														free($2);
														$$ = node;    
													}
		| func_expr									{
														$$ = $1; 
													}
		;
Sconst:
		SCONST										{ $$ = $1;}
		;			
Iconst:
		ICONST 										{ $$ = $1;}
		;
func_expr:
		IDENT '(' ')'								{
														ASTNode* funcName = new ASTNode($1) ;
														Function* func = new Function(funcName, NULL);
														free($1) ;
														$$ = func; 
													}
		| IDENT '(' func_arg_list ')'				{
														ASTNode* funcName = new ASTNode($1) ;
														Function* func = new Function (funcName, $3);
														free($1);
														$$ = func; 
													}
		| IDENT '(' '*' ')'							{
														ASTNode* funcName = new ASTNode($1);
														ASTNode* funcarg = new ASTNode ("?") ;
														FunctionArgList* args = new FunctionArgList(funcarg) ;
															
														Function* func = new Function (funcName, args);
														free($1);
														$$ = func; 
													}

		| '(' ')'									{//anonyous function
														ASTNode* funcName = new ASTNode("");
														Function* func = new Function (funcName, NULL) ;
														$$ = func; 	
													}

		| '(' func_arg_list ')'						{
														ASTNode* funcName = new ASTNode("") ;
														Function* func = new Function (funcName, $2) ;
														$$ = func; 
													}
		;

func_arg_list:
		func_arg_list ',' func_arg_expr				{
														FunctionArgList* args (NULL);
														if ($1)
															args = (FunctionArgList*) $1; 
														else 	
															args = new FunctionArgList () ; 
													
														args->addArg ($3) ;
														$$ = args;
													}
		| func_arg_expr 							{
														FunctionArgList* args = new FunctionArgList($1) ;
														$$ = args; 
													}
		;

func_arg_expr:	
		a_expr 										{ $$ = $1; }
		;	

AlterStmt:
		ALTER TABLE a_expr AlterStmt_Opts			{
														AlterStmt* alter = new AlterStmt (AlterStmt::OBJ_TABLE, $3, $4);
														$$ = alter;
													}
		;
AlterStmt_Opts:
		AlterStmt_Opts ',' AlterStmt_Opt			{
														AlterOptList* optList = $1; 
														optList->addOpt ($3);
														$$ = optList;
													}
		|	AlterStmt_Opt							{
														AlterOptList* optList = new AlterOptList();
														optList->addOpt ($1); 
														$$ = optList;
													}
		;
AlterStmt_Opt:
		ADD_P a_expr								{
														AlterOpt* opt = new AlterOpt (AlterOpt::ACT_ADD, AlterOpt::OPT_EVERY_TYPE, $2);
														$$ = opt;
													}
		| ADD_P COLUMN a_expr 						{
														AlterOpt* opt = new AlterOpt (AlterOpt::ACT_ADD, AlterOpt::OPT_COLUMN, $3);
														$$ = opt;
													}
		| ADD_P FOREIGN KEY_P a_expr REFERENCES a_expr{
                                                        AlterOpt* opt = new AlterOpt (AlterOpt::ACT_ADD, AlterOpt::OPT_FOREIGN_KEY, $4);
                                                        $$ = opt;															
													}
		| DROP a_expr								{
														AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_EVERY_TYPE, $2);
														$$ = opt; 
													}
		| DROP COLUMN a_expr						{
														AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_COLUMN, $3);	
														$$ = opt; 
													}
		| DROP FOREIGN KEY_P a_expr					{
                                                        AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_FOREIGN_KEY, $4);
                                                        $$ = opt;															
													}
		| DROP PRIMARY KEY_P a_expr					{
                                                        AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_PRIMARY_KEY,$4);
                                                        $$ = opt;															
													}
		| DROP CONSTRAINT a_expr					{
                                                        AlterOpt* opt = new AlterOpt (AlterOpt::ACT_DEL, AlterOpt::OPT_CONSTRAINT,$3);
                                                        $$ = opt;															
													}

		; 	

UpdateStmt:
		UPDATE a_expr SET set_expr_list from_stmt where_current_stmt	
													{
														UpdateStmt* update = new UpdateStmt ();
														update->setOrigTable ($2); 
														update->setExpr($4);
														update->setFromStmt($5);
														update->setWhereStmt ($6);
														$$ = update; 		
													}
			;
set_expr_list: 
		set_clause									{
														ExprList* exprs = new ExprList ();
														exprs->addExpr ($1);
														$$ = exprs; 
													}
		| set_expr_list ',' set_clause				{
														ExprList* exprlist (NULL) ;
														if ($1) 
															exprlist = (ExprList*)$1; 
														 else 
															exprlist  = new ExprList () ;
														
														exprlist->addExpr ($3) ;
														$$ = exprlist; 
													}
		;
set_clause:
		IDENT '=' compare_expr						 {
														ASTNode* id = new ASTNode($1);
														Operator* op = new Operator ("=");
														ArithExpr* eq = new ArithExpr (id, $3, op);
														free($1);
														$$ = eq; 
													
													}
		| IDENT '=' a_expr							{
                                                        ASTNode* id = new ASTNode($1);
                                                        Operator* op = new Operator ("=");
                                                        ArithExpr* eq = new ArithExpr (id, $3, op);
                                                        free($1);
                                                        $$ = eq;														
													}
		;
where_current_stmt:
		WHERE condition_expr 						{
                                                        WhereStmt* where = new WhereStmt ();
                                                        where->setExpr ($2) ;
                                                        $$ = where;														
													}
		| WHERE CURRENT_P OF_P a_expr 				{
                                                        WhereStmt* where = new WhereStmt ();
                                                        where->setExpr ((Expr*)$4) ;
                                                        $$ = where;															
													}
		| /*without where stmt*/					{
														$$ = NULL;
													}
		;
DeleteStmt:
		DELETE FROM a_expr where_stmt 				{
														DeleteStmt* del = new DeleteStmt ();
														del->setOrigTable ($3);
														del->setWhereStmt ($4);
														$$ = del;
													}
		;
InsertStmt:
		INSERT insert_target_stmt insert_values_stmt 
													{
														InsertStmt* insert = new InsertStmt ();
														insert->setDestCols($2);	
														insert->setValues($3);
														$$ = insert; 
													}
		;	
insert_target_stmt:
		INTO a_expr									{
														ASTNode* node = $2; 
														node->getNodeType();
														$$ = $2; 
													}
		;
insert_values_stmt:
		VALUES	a_expr_list							{
														$$ = $2 ;
													}
	
		;
DropStmt:
		DROP DATABASE a_expr						{
														DropStmt* drop = new DropStmt(DropStmt::ACT_DATABASE, $3);
														$$ = drop;
													}
		| DROP USER a_expr							{
														DropStmt* drop = new DropStmt(DropStmt::ACT_USER, $3);
														$$ = drop;
													}
		| DROP ROLE a_expr 							{
														DropStmt* drop = new DropStmt(DropStmt::ACT_ROLE, $3);
														$$ = drop;
													}
		;

SQLRuleStmt:
		CreateRuleStmt								{ $$ = $1; } 
		| UpdateRuleStmt 							{ $$ = $1; }
		| DropRuleStmt								{ $$ = $1; }
		;

CreateRuleStmt:
		CREATE RULE a_expr AS stmt WITH ActionOptStmt
													{
														CreateRuleStmt* createRule = new CreateRuleStmt ($3, $5, $7);
														$$ = createRule; 		
													}
		;
UpdateRuleStmt:
		UPDATE RULE a_expr AS stmt WITH ActionOptStmt
													{
														UpdateRuleStmt* updateRule = new UpdateRuleStmt ($3, $5, $7);
														$$ = updateRule; 
													}
		;
DropRuleStmt:
		DROP RULE a_expr 		 					{
														DropRuleStmt* dropRule = new DropRuleStmt ($3) ;
														$$ = dropRule;
													}
		;
ActionOptStmt:
		IGNORE										{
														ConstValue* opt = new ConstValue ($1);
														$$ = opt; 
													}
		| KILLSESSION								{
														ConstValue* opt = new ConstValue ($1) ;
														$$ = opt; 
													}
		; 
OptionStmt:
		/*empty*/									{
														/**/
													}
		| AliasStmt 								{	
														$$ = $1; 
													}
		;
AliasStmt:
		AS IDENT									{
														ASTNode* node = new ASTNode ($2);
														OptAliasStmt* aliasStmt = new OptAliasStmt (node, true);
														free ($2);
														$$ = aliasStmt; 	
													}
		;
%%
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
