/********************************************************************************
* the raw syntax tree nodes definition. In this file we give the definitions of AST
* elements. SELECT clause, WHERE clause,etc.
*********************************************************************************/
#ifndef __NODES_TYPES_H__
#define __NODES_TYPES_H__

#include <string>
#include <list>
#include <set>

#include <types.h>
#include <system.h>
#include <stdlib.h>
#include <string.h>
#include <my_list.h>

using std::string; 

class ASTNode /*: public NonCopyable*/ {
public:
	typedef enum node_type_t {
		NODE_TYPE_NONE =0,
		NODE_TYPE_NODE, 

		NODE_TYPE_SQL,

		NODE_TYPE_TARGETENTRY,
		NODE_TYPE_TARGETENTRY_LIST,

		NODE_TYPE_TABLEENTRY,
		NODE_TYPE_TABLEENTRY_LIST,	

		NODE_TYPE_SELECT_STMT, 
		NODE_TYPE_TARGETLIST_STMT,
		NODE_TYPE_FROM_STMT, 
		NODE_TYPE_WHERE_STMT, 

		NODE_TYPE_AGGREGATE_STMT,
		NODE_TYPE_HAVING_STMT,
		NODE_TYPE_GROUPBY_STMT, 
		NODE_TYPE_ORDERBY_STMT,
		NODE_TYPE_LIMIT_STMT,
	
		NODE_TYPE_JOIN_STMT,
		NODE_TYPE_JOIN_MERGE,
		NODE_TYPE_JOIN_HASH,
		NODE_TYPE_JOIN_NSETLOOP,

		NODE_TYPE_PLAN,
		NODE_TYPE_RESULT_PLAN,
		NODE_TYPE_PROJECTSET_PLAN,
		NODE_TYPE_MODIFYTABLE_PLAN,
		NODE_TYPE_APPEND_PLAN,
		NODE_TYPE_MERGEAPPEND_PLAN,
		NODE_TYPE_RECURSIVEUNION_PLAN,
		NODE_TYPE_BITMAPAND_PLAN, 
		NODE_TYPE_BITMAPOR_PLAN,

		NODE_TYPE_SCAN_PLAN,
		NODE_TYPE_SEQSCAN_PLAN,
		NODE_TYPE_SAMPLESCAN_PLAN,
		NODE_TYPE_INDEXSCAN_PLAN,		

		NODE_TYPE_FUNCTION,
		NODE_TYPE_FUNCTION_ARGLIST,
		NODE_TYPE_FUNCTION_SUM,
		NODE_TYPE_FUNCTION_AVG,
		
		NODE_TYPE_EXPR, 
		NODE_TYPE_A_EXPR, 
		NODE_TYPE_LOGICAL_EXPR,
		NODE_TYPE_ARITH_EXPR, 
		NODE_TYPE_EXPR_AND,
		NODE_TYPE_EXPR_OR,

		NODE_TYPE_OPER,
		NODE_TYPE_AND_OPER,
		NODE_TYPE_OR_OPER,
		NODE_TYPE_ADD_OPER,
		NODE_TYPE_SUB_OPER,
		NODE_TYPE_MULTI_OPER,
		NODE_TYPE_DIVIDE_OPER,		
		NODE_TYPE_BETWEEN_OPER,		
		NODE_TYPE_IN_OPER,		
		
		NODE_TYPE_CONST,
		NODE_TYPE_INT_CONST,
		NODE_TYPE_FLOAT_CONST,
		NODE_TYPE_STR_CONST,		
		NODE_TYPE_BIN_CONST,
		NODE_TYPE_X_CONST,

		NODE_TYPE_ALTER_STMT,		
		NODE_TYPE_ALTER_TABLE_OPT_LIST,
		NODE_TYPE_ALTER_TABLE_OPT,
	
		NODE_TYPE_INSERT_STMT,		
		NODE_TYPE_DELETE_STMT,	
		NODE_TYPE_UPDATE_STMT,
		NODE_TYPE_DROP_STMT,
		
		NODE_TYPE_CREATERULE_STMT,
		NODE_TYPE_UPDATERULE_STMT,
		NODE_TYPE_DROPRULE_STMT,	
	
		NODE_TYPE_EXECUTOR,	
		NODE_TYPE_RESERVED_1,
		NODE_TYPE_RESERVED_2,
		NODE_TYPE_RESERVED_N
	} NodeType;

	ASTNode() { 
		memset(name_, 0, sizeof(name_)); 
		nodeType_ = NODE_TYPE_NODE ;
	}
	ASTNode(const char* name) {
		memset (name_, 0x0 , sizeof (name_)) ;
		strcpy (name_, name) ;
		nodeType_ = NODE_TYPE_NODE ;  
	}
	virtual ~ASTNode() {}
	ASTNode(NodeType type) {
		nodeType_ = type; 
		memset(name_, 0, sizeof(name_)) ;
	}
	
	NodeType getNodeType ()const  {return nodeType_;}
	virtual void release(){}
	virtual const char* toString() {
		return name_;
	}
	virtual const char* Text() {
		return name_ ;
	}	
	virtual const char* getName() {
		return name_;
	}

private:
	NodeType nodeType_;
	char name_[128];
} ;


/*USED FOR INDICATES THE PHRASE OF SQL PROCESSING*/
typedef enum SQLPhrase{
	SQLPHRASE_UNTOUCHED = 0,
	/*FOR ANALYZE*/
	SQLPHRASE_BEFORE_ANALYZE ,
	SQLPHRASE_IN_ANALYZE,
	SQLPHRASE_AFTER_ANALYZEA,
		
	/*FOR REWRITEING*/
	SQLPHRASE_BEFORE_REWRITE,
	SQLPHRASE_IN_REWRITE,
	SQLPHRASE_AFTER_REWRITE,

	/*FOR*/
	SQLPHRASE_BEFORE_PLANNING,
	SQLPHRASE_IN_PLANNING,
	SQLPHRASE_AFTER_PLANNING,

	SQLPHRASE_RESERVED	
} SQLPhrase;

typedef enum SQLStmtType {
	STMT_UNKNOWN_TYPE=0,
	STMT_SQL_TYPE,
	STMT_SELECT_TYPE,
	STMT_INSERT_TYPE,
	STMT_UPDATE_TYPE,
	STMT_DEL_TYPE,
	STMT_ALTER_TYPE,	
	STMT_DROP_TYPE, 
	
	STMT_RESERVED_TYPE
} SQLStmtType ;

class  SqlStmt :  public ASTNode {
public:
	SqlStmt ():ASTNode(NODE_TYPE_SQL),  phrase_(SQLPHRASE_UNTOUCHED), type_(STMT_SQL_TYPE), parent_((ASTNode*)NULL){

	}
	explicit
	 SqlStmt (NodeType type) : ASTNode(type),  phrase_(SQLPHRASE_UNTOUCHED), type_(STMT_SQL_TYPE), parent_((ASTNode*)NULL){

	}
	explicit 
	SqlStmt (ASTNode* parent):ASTNode(NODE_TYPE_SQL), phrase_(SQLPHRASE_UNTOUCHED), type_(STMT_SQL_TYPE),parent_(parent) {
	
	}
	
	SqlStmt (ASTNode* parent, SQLStmtType stmtType):ASTNode(NODE_TYPE_SQL), phrase_(SQLPHRASE_UNTOUCHED), type_(STMT_SELECT_TYPE),parent_(parent) {
	
	}
	SqlStmt (NodeType type, SQLStmtType stmtType) : phrase_(SQLPHRASE_UNTOUCHED), type_(stmtType), parent_(NULL), ASTNode(type)
	{
	}	
	explicit 
	SqlStmt (SQLStmtType type) : ASTNode(NODE_TYPE_SQL), phrase_(SQLPHRASE_UNTOUCHED), type_(STMT_SELECT_TYPE),parent_(NULL) {
	}
	virtual ~SqlStmt () {}

	virtual void optimize () = 0;
	virtual const char* toString() = 0;
	virtual void release () = 0;
	
	virtual ASTNode* getParent() const {return parent_;}

	SQLPhrase getSQLPhrase() const { return phrase_;}
	SQLStmtType getSQLStmtType () const { return type_;}
	
private:
	SQLPhrase phrase_ ;
	SQLStmtType type_;
	ASTNode* parent_; 
} ;


//The placeholder. ?
class  PlaceHolder : public SqlStmt {
public:
	PlaceHolder ();
	explicit 
	PlaceHolder (ASTNode* parent);
	virtual ~PlaceHolder();
	
	virtual void optimize();
	virtual const char* toString();	
	virtual void release ();
} ;

class  TargetEntry : public SqlStmt {
public:
	TargetEntry();
	TargetEntry(ASTNode* target, ASTNode* alias) ;
	virtual ~TargetEntry ();
	
	virtual void optimize () ;
	virtual const char* toString (); 
	virtual void release ();
private:
	ASTNode* target_;
	ASTNode* alias_;
	string sqlText_ ;
} ;

class  TargetList : public SqlStmt {
public:
	TargetList ();
	virtual ~TargetList ();

	virtual void optimize (); 
	virtual void addEntry (TargetEntry* entry) ;
	
	virtual void mergeEntries(TargetList* targetList) ;
	virtual void mergeEntries(TargetEntry* entry) ;

	virtual uint32 getEntryNum () const { return list_length(entries_);};
	virtual const char* toString ();
	virtual void release ();
private: //Here, we will use the a vector to store entries.
	LIST* entries_;	
	string sqlText_;
} ;

class  TableEntry : public SqlStmt {
public:
	TableEntry ();
	TableEntry (ASTNode* tableName, ASTNode* alias) ;
	virtual ~TableEntry ();
	
	virtual void optimize();	
	virtual const char* getName () const ;
	virtual const char* toString ();
	virtual void release ();
private:
	//The length of table name will be read from conf in future.
	//static const uint32 max_length_table_name= 128; 
	string sqlText_ ;
	ASTNode* tableName_;
	ASTNode* alias_; 
};

class  TableEntryList : public SqlStmt {
public:
	TableEntryList();
	virtual ~TableEntryList();
	
	virtual void addTableEntry (TableEntry* table) ;
	virtual void mergeTableList (TableEntryList* table_list) ;
	virtual uint32 getTableNum () const { return list_length(tables_);}	

	virtual void optimize();
	virtual const char* toString();
	virtual void release (); 
private:
	LIST* tables_;
	string sqlText_; 
};

class  IntoStmt : public SqlStmt {
public:
	IntoStmt ();
	virtual ~IntoStmt();

	virtual void optimize();
	virtual const char* toString();
	virtual void release ();
private:
	string sqlText_ ;
} ;
class  FromStmt : public SqlStmt {
public:
	FromStmt ();
	virtual ~FromStmt ();
	
	virtual void setTableEntryList (TableEntryList* tables);
	virtual void setTableEntryList (ASTNode* tables);
	virtual void optimize ();
	virtual const char* toString();
	virtual void release ();
private:
	TableEntryList* tables_;	
	string sqlText_ ;
} ;


class OrderByStmt;
class GroupByStmt;
class HavingStmt;
class LimitStmt; 

class  AggregateStmt : public SqlStmt {
public:
	AggregateStmt() ;
	virtual ~AggregateStmt ();

	virtual void optimize ();
	virtual const char* toString();
	virtual void release();
	
	virtual void setOrderByStmt (OrderByStmt* order) { order_ = order; }
	virtual OrderByStmt* getOrderByStmt() const { return order_;}

	virtual void setGroupByStmt (GroupByStmt* group) {group_= group;}
	virtual GroupByStmt* getGroupByStmt() const { return group_;}

	virtual void setHavingStmt(HavingStmt* having) {having_ = having;}
	virtual HavingStmt* getHavingStmt() const { return having_ ;}

	virtual void setLimitStmt(LimitStmt* limit) {limit_ = limit;}
	virtual LimitStmt* getLimitStmt() const { return limit_ ;}

protected:
	explicit AggregateStmt(NodeType type);
private:	
	OrderByStmt* order_; 
	GroupByStmt* group_;
	HavingStmt* having_;
	LimitStmt*  limit_;

	string sqlText_; 
} ;

class LimitStmt : public AggregateStmt {
public:
	explicit 
	LimitStmt (ASTNode* expr);
	LimitStmt ();
	virtual ~LimitStmt();

	virtual void optimize();
	virtual const char* toString(); 
	virtual void release();
private:
	ASTNode* expr_;
	string sqlText_ ;
} ;

class  HavingStmt : public AggregateStmt {
public:
	HavingStmt();
	explicit 
	HavingStmt(ASTNode* node);
	virtual ~HavingStmt();

	virtual void optimize ();	
	virtual const char* toString();
	virtual void release();
private:
	ASTNode* havingExpr_;
	string sqlText_;
} ;

class  GroupByStmt : public AggregateStmt {
public:
	GroupByStmt ();
	explicit 
	GroupByStmt (ASTNode* node);
	virtual ~GroupByStmt ();

	virtual void optimize (); 
	virtual const char* toString();
	virtual void release();
private:
	ASTNode* groupExpr_ ;
	string sqlText_;
};

class  OrderByStmt : public AggregateStmt {
public:
	OrderByStmt();
	explicit 
	OrderByStmt(ASTNode* orderExpr);
	virtual ~OrderByStmt();

	virtual void optimize();
	virtual const char* toString();
	virtual void release();
private:
	ASTNode* orderExpr_;
	string sqlText_;
};

class FunctionArgList : public SqlStmt {
public:
	FunctionArgList();
	explicit 
	FunctionArgList (ASTNode* arg) ;
	virtual ~FunctionArgList () ;

	virtual void addArg (ASTNode* arg);
	virtual void mergeArgList (FunctionArgList* argList);

	virtual void optimize();
	virtual void release();
	virtual const char* toString();
private:
	LIST* args_ ; 
	string sqlText_;
} ;
class  Function : public SqlStmt {
public:
	Function ();
	Function (ASTNode* name, FunctionArgList* args);
	virtual ~Function ();
		
	virtual void optimize ();
	virtual const char* toString();
	virtual void release();
	
	virtual ASTNode* getFuncName () const {return name_;}
	virtual FunctionArgList* getFuncArgs () const {return args_;}
protected:
	explicit Function(NodeType type) ;
private:
	ASTNode* name_;
	FunctionArgList* args_;
	string sqlText_; 
} ;


class  Sum : public Function {
public:
	Sum(); 
	virtual ~Sum ();

	virtual void optimize();
	virtual const char* toString();
	virtual void release();
} ;

class  Avg : public Function {
public:
	Avg ();
	virtual ~Avg (); 
	
	virtual void optimize();
	virtual const char* toString();
	virtual void release();
};

class  Operator : public SqlStmt {
public:
	//The type used to determine whether oper is legal or not.
	typedef enum OperatorType{
			unary_operator, //unary operator. such as '-'
			binary_operator,//binary operator, such as '+'/'-', etc. 
			triple_operator, //such as '?:', 'between and'
			
			multi_opator //others
	} OperatorType;
	Operator ();
	explicit 
	Operator (const char* operName) ;
	virtual ~Operator ();
	
	virtual void optimize ();
	virtual const char* toString();
	virtual void release();
protected:
	Operator (NodeType type);
	Operator (NodeType type, const char* operName);
private:
	string sqlText_; 
	ASTNode* family_;
};

class BetweenAnd : public Operator {
public:
	BetweenAnd();
	BetweenAnd(ASTNode* oper, const char* operName);
	virtual ~BetweenAnd (); 

	virtual void optimize ();
	virtual void release();
	virtual const char* toString ();
	virtual ASTNode* getOperator() const {return operator_;}
private:
	ASTNode* operator_;	
};

class InOper : public Operator {
public:
	InOper();
	InOper(ASTNode* left_, ASTNode* right);
	virtual ~InOper (); 

	virtual void optimize ();
	virtual void release();
	virtual const char* toString ();
private:
	ASTNode* left_, *right_;	
	string sqlText_ ;
};

//The definit of executable experssion clause.
class  Expr : public SqlStmt {
public:
	Expr () ; 
	virtual ~Expr() ;

	virtual void optimize ();
	virtual const char* toString();
	virtual void release();
protected:
	explicit Expr(NodeType type);
};

class LogicalExpr : public Expr{
public:
	LogicalExpr ();
	LogicalExpr (ASTNode* left, ASTNode* right, Operator* oper) ;	
	virtual ~LogicalExpr ();

	virtual void optimize();
	virtual void release();
	virtual const char* toString();
protected:
	LogicalExpr (NodeType type) ;
private:
	ASTNode* left_, * right_;
	Operator* oper_;
	string sqlText_ ; 
} ;

class ArithExpr : public Expr {
public:
	ArithExpr ();
	ArithExpr (ASTNode* left, ASTNode* right, Operator* oper);
	virtual ~ArithExpr ();
	
	virtual void optimize();
	virtual const char* toString();
	virtual void release();
protected:
	explicit ArithExpr (NodeType type);
private:
	ASTNode* left_, *right_;
	Operator* oper_;
	string sqlText_;
};
	
class  And : public LogicalExpr {
public:
	And () ;
	virtual ~And () ;
	
	And (ASTNode* left, ASTNode* rigth) ;	
	virtual ASTNode* Result () ; 
	
	virtual Expr* reverse ();
	virtual void optimize ();
	virtual const char* toString();
	virtual void release();

	virtual ASTNode* getLeftOperand () const { return left_; }
	virtual ASTNode* getRightOperand () const {return right_;}
private:
	ASTNode* left_, *right_ ;
	string sqlText_;
} ;

class  Or : public LogicalExpr {
public:
	Or();
	virtual ~Or();
	Or (ASTNode* left, ASTNode* right) ;
	
	virtual Expr* reverse ();
	virtual void optimize (); 
	virtual const char* toString();
	virtual void release();
	virtual ASTNode* Result(); 
	
	virtual ASTNode* getLeftOperand () const {return left_ ; }
	virtual ASTNode* getRightOperand () const { return right_ ; }
private:
	ASTNode* left_, *right_;
	string sqlText_ ;
} ;

class  WhereStmt : public SqlStmt {
public:
	WhereStmt () ;
	virtual ~WhereStmt (); 
		
	virtual void optimize ();
	virtual void setExpr (Expr* expr) { expr_ =  expr;}
	virtual Expr* getExpr () const {return expr_;}

	virtual const char* toString();
	virtual void release();
private:
	Expr* expr_ ;
	string sqlText_;
} ;

class ConstValue : public SqlStmt {
public:
	ConstValue () ;
	explicit 
	ConstValue (int value) ;

	explicit
	ConstValue (double value) ;
	
	explicit
	ConstValue (const char* value);
	virtual ~ConstValue ();
	
	virtual void optimize();
	virtual const char* toString();
	virtual void release();
	virtual const char* Text();
private:
	string sqlText_ ;
} ;


class AlterOpt : public SqlStmt {
public:
    typedef enum {
        ACT_ADD,
        ACT_DEL,
        ACT_RENAME
    } ACT_TYPE;
	
	typedef enum {
		OPT_EVERY_TYPE,
		OPT_COLUMN,
		OPT_INDEX,
		OPT_KEY,
		OPT_CONSTRAINT,
		OPT_PRIMARY_KEY,
		OPT_FOREIGN_KEY
	}OPT_TYPE;
	
    AlterOpt ();

	explicit
    AlterOpt (ASTNode* obj);
    AlterOpt (ACT_TYPE actType,OPT_TYPE optType, ASTNode* obj);
    virtual ~AlterOpt ();

    virtual void optimize ();
    virtual void release();
    virtual const char* toString();

private:
	OPT_TYPE opt_type_;	
    ACT_TYPE actType_;
    ASTNode* obj_ ;
	string sqlText_;
};

class AlterOptList : public SqlStmt {
public:
    AlterOptList();

	explicit
    AlterOptList (AlterOpt* opt);
    virtual ~AlterOptList();

    virtual void optimize();
    virtual void release();
    virtual const char* toString();
    virtual void mergeOptList(AlterOptList* other);
    virtual void addOpt (AlterOpt*);
private:
    //the type of dest object.
    LIST* options_;
	string sqlText_;
};



class  SelectStmt : public SqlStmt { 
public:
	SelectStmt ();
	explicit 
	SelectStmt (ASTNode* parent);
	
	explicit 
	SelectStmt (SelectStmt* parent);
	virtual ~SelectStmt(); 

	virtual void optimize ();
	
	virtual inline void setTargetList (TargetList* targetList) { targetList_ = targetList; }
	virtual inline TargetList* getTargetList () const { return targetList_;}

	virtual inline void setIntoStmt(IntoStmt* intoStmt) {intoStmt_ = intoStmt;}
	virtual inline IntoStmt* getIntoStmt() const {return intoStmt_;}
	
	virtual inline void setFromStmt(FromStmt* fromStmt) { fromStmt_= fromStmt; }
	virtual inline FromStmt* getFromStmt () const { return fromStmt_;}

	virtual inline void setWhereStmt (WhereStmt* whereStmt) { whereStmt_ = whereStmt;}
	virtual inline WhereStmt* getWhereStmt () const {return whereStmt_;}

	virtual inline void setAggStmt (AggregateStmt* agg) { aggStmt_ = agg;}
	virtual inline AggregateStmt* getAggStmt () const { return aggStmt_ ;}

	virtual const char* toString();
	virtual void release();
private:
	TargetList*     targetList_;
	IntoStmt* 		intoStmt_;
	FromStmt*       fromStmt_; 
	WhereStmt*      whereStmt_; 
	AggregateStmt*  aggStmt_;	
	ASTNode* 		parent_;
	
	string sqlText_; 
};


class AlterStmt : public SqlStmt {
public:
    typedef enum {
        OBJ_TABLE,
        OBJ_VIEW,
        OBJ_INDEX
    } OBJ_TYPE;

	AlterStmt();
	AlterStmt(OBJ_TYPE objType, ASTNode* relName, AlterOptList* options);
	virtual ~AlterStmt();
	
	virtual void optimize();
	virtual void release();
	virtual const char* toString();

private:
	OBJ_TYPE objType_;
	ASTNode* relName_ ;
	AlterOptList* options_;
	string sqlText_ ;
};

class ExprList : public Expr 
{
public:
	ExprList ();
	virtual ~ExprList ();
	
	virtual void optimize();
	virtual void release();
	virtual const char* toString();
	
	virtual void addExpr (Expr* expr);
	virtual void mergeList (ExprList* other);
private:
	LIST* exprs_;	
	string sqlText_;
};

class A_ExprList : public ASTNode
{
public:
	A_ExprList ();
	virtual ~A_ExprList ();
	
	virtual void optimize();
	virtual void release();
	virtual const char* toString();
	
	virtual void addExpr (ASTNode* expr);
	virtual void mergeList (A_ExprList* other);
private:
	LIST* exprs_;	
	string sqlText_;
};

class UpdateStmt : public SqlStmt {
public:
	UpdateStmt ();
	virtual ~UpdateStmt ();
	
	virtual void setOrigTable(ASTNode* orig) { orig_ = orig;}
	virtual void setExpr (ExprList* expr) {expr_= expr; }
	virtual void setWhereStmt (WhereStmt* where) {where_ = where;}
	virtual void setFromStmt (FromStmt* from ) {from_ = from;}
	
	virtual ASTNode* getOrigTable () const {return orig_;}
	virtual ExprList* getExpr () const {return expr_;}
	virtual FromStmt* getFromStmt() const {return from_;}
	virtual WhereStmt* getWhereStmt () const {return where_;}
	
	virtual void optimize();
	virtual void release();
	virtual const char* toString();
private:
	ASTNode* orig_; 
	ExprList* expr_; 
	FromStmt* from_;
	WhereStmt* where_; 
	string sqlText_;
};

class DeleteStmt : public SqlStmt {
public:
	DeleteStmt ();
	virtual ~DeleteStmt (); 

	virtual void optimize (); 
	virtual void release (); 
	virtual const char* toString (); 
	
	virtual void setOrigTable (ASTNode* orig) { orig_ = orig ; }
	virtual ASTNode* getOrigTable () const {return orig_; }
	virtual void setWhereStmt (WhereStmt* where) {where_ = where; }
	virtual WhereStmt* getWhereStmt () const {return where_ ;}
private:
	ASTNode* orig_; 
	WhereStmt* where_; 
	string sqlText_;
};

class InsertStmt : public SqlStmt {
public:
	InsertStmt();
	virtual ~InsertStmt ();

	virtual void optimize();
	virtual void release();
	virtual const char* toString();
	
	virtual void setDestCols (ASTNode* cols) {cols_ = cols;} 
	virtual ASTNode* getDestCols () const {return cols_;} 
	virtual void setValues (A_ExprList* values) {values_ = values; }
	virtual A_ExprList* getValues()const {return values_;}
private:
	ASTNode* cols_; 
	A_ExprList *values_; 
	string sqlText_; 
};

class DropStmt : public SqlStmt {
public:
	typedef enum {
		ACT_NONE,
		ACT_DATABASE,
		ACT_TABLE, 
		ACT_INDEX,
		ACT_KEY,
		ACT_ROLE,
		ACT_USER,
		ACT_RESERVED
	} ACT_TYPE; 
	
	DropStmt();
	DropStmt(ACT_TYPE type, ASTNode* obj) ;
	virtual ~DropStmt ();
		
	virtual void optimize ();
	virtual void release();
	virtual const char* toString();
private:
	ACT_TYPE act_type_;
	ASTNode* obj_; 
	string sqlText_ ;
};

class CreateRuleStmt : public SqlStmt {
public:
	CreateRuleStmt(ASTNode* sql, ASTNode* pattern, ASTNode* sqlAction) ;
	virtual ~CreateRuleStmt ();
		
	virtual void optimize ();
	virtual void release ();
	virtual const char* toString ();
		
	virtual int execute ();
	virtual const char* sqlText() ;
	virtual const char* sqlPattern(); 
	virtual const char* sqlAction();
private:
	ASTNode* sql_, *pattern_, *sqlAction_;
};

class UpdateRuleStmt : public SqlStmt {
public:
	UpdateRuleStmt (ASTNode* sql, ASTNode* pattern, ASTNode* action) ;
	virtual ~UpdateRuleStmt () ;

	virtual void optimize();
	virtual void release ();
	virtual const char* toString ();
	
	virtual int execute (); 
	virtual const char* sqlText(); 
	virtual const char* sqlPattern();
	virtual const char* sqlAction();
private:
	ASTNode* sql_, *pattern_, *action_;
};

class DropRuleStmt : public SqlStmt {
public:
	DropRuleStmt (ASTNode* sql) ;
	virtual ~DropRuleStmt ();

	virtual void optimize ();
	virtual void release() ;
	virtual const char* toString();
	
	virtual int execute ();
	virtual const char* sqlText();
private:
	ASTNode* sql_ ;
};
#endif //__NODES_TYPES_H__
