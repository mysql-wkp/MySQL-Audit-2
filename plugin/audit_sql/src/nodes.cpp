/**
	the definition of nodes types.
	path:src/types/nodes.cpp
**/
#if MYSQL_VERSION_ID <= 50700
#include "my_global.h"
#include <my_dbug.h>
#else
#include <my_dbug.h>
#endif 
#include <my_sys.h>

#if MYSQL_VERSION_ID <= 50700
#else
#include <include/mysql/psi/psi_memory.h>
#endif 

#include <stdlib.h>
#include <string.h>
#include <nodes.h>

#if MYSQL_VERSION_ID <= 50700
#else
PSI_memory_key audit_ast_memory_LIST ; 
#endif 

PlaceHolder::PlaceHolder()
{
}
PlaceHolder::PlaceHolder (ASTNode* parent)
{
}
PlaceHolder::~PlaceHolder()
{
}
void PlaceHolder::optimize()
{}
const char* PlaceHolder::toString()
{
	return "?" ;
}
void PlaceHolder::release()
{//To release the itself.
}


SelectStmt::SelectStmt() : targetList_(NULL),intoStmt_(NULL),fromStmt_(NULL), aggStmt_(NULL), whereStmt_(NULL), parent_(NULL),SqlStmt(STMT_SELECT_TYPE)
{
}

SelectStmt::SelectStmt(ASTNode* parent) : targetList_(NULL),intoStmt_(NULL),fromStmt_(NULL),whereStmt_(NULL),aggStmt_(NULL), parent_(NULL),SqlStmt(parent, STMT_SELECT_TYPE)
{
}
SelectStmt::SelectStmt(SelectStmt* parent) : targetList_(NULL),intoStmt_(NULL),fromStmt_(NULL),whereStmt_(NULL),aggStmt_(NULL), parent_(NULL),SqlStmt(parent, STMT_SELECT_TYPE)
{
}

SelectStmt::~SelectStmt()
{
}

void SelectStmt::optimize ()
{
	DBUG_ASSERT(false);
}

const char*  SelectStmt::toString()
{
	sqlText_ = "SELECT";
	TargetList* targetList = getTargetList (); 
	if (targetList) {
		sqlText_ += " " ;	
		sqlText_ += targetList->toString();
	} else{
		sqlText_ += " N/A ";
	}

	FromStmt* from = getFromStmt();
	if (from){
		sqlText_ += from->toString();
	}

	WhereStmt* where = getWhereStmt();
	if (where) {
		sqlText_ += where->toString();
	}

	AggregateStmt* agg = getAggStmt();
	if (agg) {
		sqlText_ += agg->toString(); 
	}
	return sqlText_.c_str();
}
void SelectStmt::release()
{
	TargetList* targetList = getTargetList ();
	if (targetList){
		targetList->release();
		delete targetList ; 
		targetList = NULL;
		//setTargetList(NULL);
	}
	
	FromStmt* from = getFromStmt();
	if (from) {
		from->release();
		delete from; 
		from = NULL;
	}
	
	WhereStmt* where = getWhereStmt ();
	if (where){
		where->release();
		delete where;
		where = NULL;
	}
	
	AggregateStmt* agg = getAggStmt ();
	if (agg){
		agg->release();
		delete agg; 
		agg = NULL;
	}
			
	ASTNode* parent = getParent ();
	if (parent){
		parent->release();
		delete parent;
		parent = NULL;
	}
}

TargetEntry::TargetEntry() : target_(NULL), alias_ (NULL),SqlStmt (NODE_TYPE_TARGETENTRY)
{
}

TargetEntry::TargetEntry(ASTNode* target, ASTNode* alias) : target_(target), alias_(alias), SqlStmt (NODE_TYPE_TARGETENTRY)
{
}
TargetEntry::~TargetEntry()
{
}

void TargetEntry::optimize()
{
	DBUG_ASSERT(false);
}

const char* TargetEntry::toString()
{
	if (!target_) //if there is no target entry in, we assume that is '?'.
		return "?";

	switch (target_->getNodeType()) {
		case NODE_TYPE_FUNCTION : {
			sqlText_.assign (target_->toString());
		} break ; 
	
		case NODE_TYPE_TARGETENTRY :
		case NODE_TYPE_TARGETLIST_STMT:
		case NODE_TYPE_TARGETENTRY_LIST:
		default : {
			sqlText_ = "?" ;
		} break; 
	}
	
	return sqlText_.c_str();
}
void TargetEntry::release()
{
	if (target_){
		target_->release();
		delete target_; 
		target_ = NULL; 
	}
	if (alias_) {
		alias_ ->release();
		delete alias_; 
		alias_= NULL; 
	}
}

TargetList::TargetList():entries_(NULL),SqlStmt (NODE_TYPE_TARGETENTRY_LIST)
{
}
TargetList::~TargetList()
{

}
void TargetList::addEntry (TargetEntry* entry) 
{
    DBUG_ASSERT (entry->getNodeType() == NODE_TYPE_TARGETENTRY);
#if MYSQL_VERSION_ID <= 50700
    LIST* elem = (LIST*)my_malloc (sizeof (LIST), MYF(0));
#else
    LIST* elem = (LIST*)my_malloc (audit_ast_memory_LIST, sizeof (LIST), MYF(0));
#endif 
    elem->data = entry;
    entries_ = list_add (entries_, elem) ;
	
}
void TargetList::mergeEntries (TargetList* targetList) 
{
	DBUG_ASSERT(false) ;
}
void TargetList::mergeEntries(TargetEntry* entry)
{
    DBUG_ASSERT (entry->getNodeType() == NODE_TYPE_TARGETENTRY);

#if MYSQL_VERSION_ID <= 50700
    LIST* elem = (LIST*)my_malloc (sizeof (LIST), MYF(0));
#else
    LIST* elem = (LIST*)my_malloc (audit_ast_memory_LIST, sizeof (LIST), MYF(0));
#endif 
    elem->data = entry;
    entries_ = list_add (entries_, elem);
}
const char* TargetList::toString()
{
	if (!entries_)
		return ""; //entries is null, then return null;
    
	LIST* index = entries_ ;
    for (; index; index = index->next){
        TargetEntry* target = (TargetEntry*) index->data ;
		DBUG_ASSERT (target->getNodeType() == NODE_TYPE_TARGETENTRY);
		
		sqlText_ += target->toString();
		if (index->next) {
			sqlText_ += " , " ;
		}
    }
	
	return sqlText_.c_str();
}
void TargetList::release()
{
	LIST* index = entries_ ;
	for (; index; index = index->next){
        TargetEntry* target = (TargetEntry*) index->data ;
		DBUG_ASSERT (target->getNodeType() == NODE_TYPE_TARGETENTRY);
		target->release (); 
		delete target ; 
		target = NULL ;	
	}

	list_free (entries_,0) ;
}
void TargetList::optimize()
{
	DBUG_ASSERT(false);
}

TableEntry::TableEntry():tableName_(NULL), alias_(NULL), SqlStmt (NODE_TYPE_TABLEENTRY)
{
}
TableEntry::TableEntry(ASTNode* tablename, ASTNode* alias):tableName_(tablename), alias_(alias), SqlStmt (NODE_TYPE_TABLEENTRY)
{
}
TableEntry::~TableEntry()
{
}
void TableEntry::optimize()
{
	DBUG_ASSERT(false);
}
const char* TableEntry::getName () const {
	return tableName_->toString() ; 
}
const char* TableEntry::toString()
{
	if (tableName_) {
		sqlText_ += tableName_ ->toString();
	} 
	if (alias_){
		sqlText_ += " AS " ;
		sqlText_ += alias_ ->toString();	
	}
	return sqlText_.c_str();
}
void TableEntry::release()
{
	if (tableName_) {
		tableName_->release() ;
		delete tableName_ ; 
		tableName_ = NULL; 
	}
	if (alias_) {
		alias_->release();
		delete alias_ ; 
		alias_ = NULL; 
	}
}

TableEntryList::TableEntryList():tables_(NULL), SqlStmt (NODE_TYPE_TABLEENTRY_LIST)
{
}
TableEntryList::~TableEntryList()
{
}
void TableEntryList::addTableEntry (TableEntry* table) 
{
	DBUG_ASSERT(table->getNodeType() == NODE_TYPE_TABLEENTRY) ;
#if MYSQL_VERSION_ID <= 50700
	LIST* elem =(LIST*) my_malloc (sizeof (LIST), MYF(0)) ; 
#else
	LIST* elem =(LIST*) my_malloc (audit_ast_memory_LIST,sizeof (LIST), MYF(0)) ; 
#endif 
	elem->data = table;
	tables_ = list_add (tables_, elem);
}
void TableEntryList::mergeTableList (TableEntryList* table_list)
{
	DBUG_ASSERT(false) ;
}
void TableEntryList::optimize()
{
	DBUG_ASSERT(false) ;
}
const char* TableEntryList::toString()
{
	string tablename ;
	LIST* start = tables_;
	if (start) {
		TableEntry* table = (TableEntry*) start->data; 
		DBUG_ASSERT(table->getNodeType() == NODE_TYPE_TABLEENTRY);
		tablename = table->toString() ;
		
		sqlText_ = tablename + sqlText_ ;
	}

	start = start->next; 
	for (;start; start= start->next) { //travel through list. to release all the table objs.
		TableEntry* table = (TableEntry*) start->data; 
		DBUG_ASSERT(table->getNodeType() == NODE_TYPE_TABLEENTRY);
		tablename = table->toString() ;

		if (start->next)
			tablename += " , " ; 
 	
		sqlText_ = tablename + sqlText_ ;
	}

	sqlText_ = " " + sqlText_ ;
	return sqlText_.c_str();
}
void TableEntryList::release()
{
	LIST* start = tables_;
	for (;start; start= start->next) { //travel through list. to release all the table objs.
		TableEntry* table = (TableEntry*)start->data; 
		DBUG_ASSERT(table->getNodeType() == NODE_TYPE_TABLEENTRY);
		table->release();		
		delete table;
		table = NULL; 
	}
	
	list_free(tables_, 0); //free the list itself.
}
IntoStmt::IntoStmt()
{
}
IntoStmt::~IntoStmt()
{
}
void IntoStmt::optimize()
{
}
const char* IntoStmt::toString()
{
	DBUG_ASSERT(false);
	return sqlText_.c_str();
}
void IntoStmt::release()
{
}
FromStmt::FromStmt():tables_(NULL),SqlStmt (NODE_TYPE_FROM_STMT)
{
}

FromStmt::~FromStmt()
{
}
void FromStmt::setTableEntryList (TableEntryList* tables)
{
	tables_ = tables;	
}
void FromStmt::setTableEntryList (ASTNode* tables)
{
	DBUG_ASSERT( tables->getNodeType() == NODE_TYPE_TABLEENTRY_LIST) ;
	tables_ = (TableEntryList*) tables; 	
}
void FromStmt::optimize()
{
	DBUG_ASSERT(false);
}

const char* FromStmt::toString()
{
	sqlText_ = " FROM "; 
	if (tables_) { //walk through list.
		sqlText_ += tables_->toString();
	}
	
	return sqlText_.c_str();
}
void FromStmt::release()
{
	if (tables_) {
		tables_->release();
		delete tables_; 
		tables_ = NULL; 
	}
}

WhereStmt::WhereStmt(): SqlStmt (NODE_TYPE_WHERE_STMT)
{
}

WhereStmt::~WhereStmt()
{
}

void WhereStmt::optimize()
{
	DBUG_ASSERT(false);
}

const char* WhereStmt::toString()
{
	
	sqlText_ = " WHERE ";
	if (expr_) {
		sqlText_ += expr_->toString();
	}

	return sqlText_.c_str();
}
void WhereStmt::release()
{
	if (expr_) {
		expr_->release();
		delete expr_ ;
		expr_ = NULL; 
	}
}

AggregateStmt::AggregateStmt() : order_(NULL), group_(NULL), having_(NULL), SqlStmt (NODE_TYPE_AGGREGATE_STMT)
{
}
AggregateStmt::AggregateStmt(NodeType type) : order_(NULL), group_(NULL), having_(NULL), SqlStmt (type)
{
}
AggregateStmt::~AggregateStmt()
{
}
void AggregateStmt::optimize()
{
	DBUG_ASSERT(false);
}
const char* AggregateStmt::toString()
{
	sqlText_=" ";
	
	if (group_) {
		sqlText_ += group_->toString() ;
		sqlText_ +=" ";
	}
	if (having_) {
		sqlText_ += having_->toString(); 
		sqlText_ += " ";
	}
	if(order_) {
		sqlText_ += order_->toString() ; 
		sqlText_ +=" " ;
	}
	if (limit_) {
		sqlText_ += limit_->toString(); 
		sqlText_ += " ";
	}
	return sqlText_.c_str();
}
void AggregateStmt::release()
{
	if (order_) {
		order_->release();
		delete order_; 
		order_ = NULL;
	}
	if (group_) {
		group_->release();
		delete group_; 
		group_ = NULL;
	}
	if (having_) {
		having_ ->release();
		delete having_;
		having_= NULL;
	}
	if (limit_) {
		limit_->release();
		delete limit_ ;
		limit_ = NULL; 
	}
}

LimitStmt::LimitStmt (): expr_ (NULL), AggregateStmt(NODE_TYPE_LIMIT_STMT)
{
}

LimitStmt::LimitStmt (ASTNode* expr): expr_ (expr), AggregateStmt(NODE_TYPE_LIMIT_STMT)
{
}
LimitStmt::~LimitStmt ()
{
}
void LimitStmt::optimize()
{
	DBUG_ASSERT(false);
}

const char* LimitStmt::toString()
{
    sqlText_ = (" LIMIT ");
    sqlText_ += expr_->toString();
    return sqlText_.c_str();
}
void LimitStmt::release()
{
	if (expr_) {
		expr_->release(); 
		delete expr_ ; 
		expr_ = NULL; 
	}
}

HavingStmt::HavingStmt() : havingExpr_(NULL), AggregateStmt(NODE_TYPE_HAVING_STMT)
{
}
HavingStmt::HavingStmt(ASTNode* node) : havingExpr_(node), AggregateStmt(NODE_TYPE_HAVING_STMT)
{
} 
HavingStmt::~HavingStmt()
{
}
void HavingStmt::optimize()
{
	DBUG_ASSERT(false);
}
const char* HavingStmt::toString()
{
	sqlText_ = ("HAVING ");
	sqlText_ += havingExpr_->toString();
	return sqlText_.c_str();
}
void HavingStmt::release()
{
	if (havingExpr_) {
		havingExpr_->release();
		delete havingExpr_ ; 
		havingExpr_ = NULL;
	}
}
GroupByStmt::GroupByStmt() :groupExpr_(NULL), AggregateStmt(NODE_TYPE_GROUPBY_STMT)
{
}
GroupByStmt::GroupByStmt (ASTNode* node) : groupExpr_(node), AggregateStmt(NODE_TYPE_GROUPBY_STMT)
{
}
GroupByStmt::~GroupByStmt()
{
}
void GroupByStmt::optimize()
{
	DBUG_ASSERT(false) ;
}
const char* GroupByStmt::toString()
{
	sqlText_ = ("GROUP BY ") ;	
	sqlText_ += groupExpr_->toString();
	return sqlText_.c_str();
}
void GroupByStmt::release()
{
	if (groupExpr_){
		groupExpr_->release();
		delete groupExpr_ ;
		groupExpr_ = NULL;
	}
}

OrderByStmt::OrderByStmt() : orderExpr_(NULL), AggregateStmt (NODE_TYPE_ORDERBY_STMT)
{
}
OrderByStmt::OrderByStmt(ASTNode* orderExpr) : orderExpr_(orderExpr), AggregateStmt (NODE_TYPE_ORDERBY_STMT)
{
}
OrderByStmt::~OrderByStmt()
{
}
void OrderByStmt::optimize()
{
	DBUG_ASSERT(false);
}
const char* OrderByStmt::toString()
{
	sqlText_ = ("ORDER BY "); 
	sqlText_ += orderExpr_->toString();	
	return sqlText_.c_str();
}
void OrderByStmt::release()
{
	if (orderExpr_) {
		orderExpr_->release();
		delete orderExpr_;
		orderExpr_ = NULL;
	}
}

FunctionArgList::FunctionArgList(): args_(NULL), SqlStmt (NODE_TYPE_FUNCTION_ARGLIST)
{
}
FunctionArgList::FunctionArgList(ASTNode* arg) : args_(NULL), SqlStmt (NODE_TYPE_FUNCTION_ARGLIST)
{
#if MYSQL_VERSION_ID <= 50700
    LIST* elem = (LIST*)my_malloc (sizeof (LIST), MYF(0));
#else
    LIST* elem = (LIST*)my_malloc (audit_ast_memory_LIST, sizeof (LIST), MYF(0));
#endif 
    elem->data = arg;
    args_ = list_add (args_, elem);

}
FunctionArgList::~FunctionArgList()
{
}
void FunctionArgList::addArg(ASTNode* arg)
{
#if MYSQL_VERSION_ID <= 50700
    LIST* elem = (LIST*)my_malloc (sizeof (LIST), MYF(0));
#else
    LIST* elem = (LIST*)my_malloc (audit_ast_memory_LIST, sizeof (LIST), MYF(0));
#endif 
    elem->data = arg;
    args_ = list_add (args_, elem);
}
void FunctionArgList::mergeArgList(FunctionArgList* args)
{
	DBUG_ASSERT(false);		
}
void FunctionArgList::optimize()
{
	DBUG_ASSERT(false);		
}
void FunctionArgList::release()
{
    LIST* index = args_ ;
    for (; index; index = index->next){
        ASTNode* node = (ASTNode*) index->data ;
        node->release();
        delete node;
		node = NULL; 
    }

    list_free (args_,0) ;
}
const char* FunctionArgList::toString()
{
	return "?"; 
}

Function::Function() :name_(NULL), args_(NULL), SqlStmt(NODE_TYPE_FUNCTION)
{
}
Function::Function(NodeType type) : name_(NULL), args_(NULL),SqlStmt(type)
{
}
Function::Function(ASTNode* name, FunctionArgList* args) : name_(name), args_(args), SqlStmt(NODE_TYPE_FUNCTION)
{
}
Function::~Function()
{
}
void Function::optimize()
{
	DBUG_ASSERT(false);
}
const char* Function::toString()
{
	if (name_) {
		sqlText_+= name_->toString();
	}
	
	sqlText_ += " ( ";
	if (args_) {
		sqlText_ += args_->toString();
	}
		
	sqlText_  += " )";
	return sqlText_.c_str();
}
void Function::release()
{
	if (name_){
		name_->release();
		delete name_;
		name_ = NULL;
	}
	if (args_) {
		args_->release();
		delete args_ ; 
		args_ = NULL; 
	}
}

Sum::Sum() : Function (NODE_TYPE_FUNCTION_SUM) 
{
}
Sum::~Sum()
{
}
void Sum::optimize()
{
	DBUG_ASSERT(false);
}
const char* Sum::toString()
{
	DBUG_ASSERT(false);
	return NULL;
}
void Sum::release()
{
}

Avg::Avg() : Function(NODE_TYPE_FUNCTION_AVG)
{
}
Avg::~Avg()
{
}
void Avg::optimize()
{
	DBUG_ASSERT(false);
}
const char* Avg::toString()
{	
	DBUG_ASSERT(false);
	return NULL;
}
void Avg::release()
{
}

Expr::Expr() : SqlStmt(NODE_TYPE_EXPR)
{
}
Expr::Expr(NodeType type) : SqlStmt(type)
{
}
Expr::~Expr()
{

}
void Expr::optimize()
{
	DBUG_ASSERT(false);
}
const char* Expr::toString()
{
	return NULL;
}
void Expr::release()
{
}

Operator::Operator(): family_(NULL),SqlStmt (NODE_TYPE_OPER)
{
}
Operator::Operator(const char* operName) :family_(NULL),SqlStmt (NODE_TYPE_OPER)
{
	sqlText_.assign(operName);
}
Operator::Operator (NodeType type) : family_(NULL), SqlStmt(type)
{
}
Operator::Operator (NodeType type, const char* operName): family_(NULL), SqlStmt(type)
{
	sqlText_.assign(operName);
}
Operator::~Operator()
{
}
void Operator::optimize()
{
	DBUG_ASSERT(false);
}
const char* Operator::toString()
{
	return sqlText_.c_str();
}
void Operator::release()
{
	if (family_){
		family_->release();
		delete family_;
		family_ = NULL;
	}
}

BetweenAnd::BetweenAnd (): operator_(NULL), Operator(NODE_TYPE_BETWEEN_OPER)
{
}
BetweenAnd::BetweenAnd(ASTNode* oper, const char* operName): operator_(oper), Operator(NODE_TYPE_BETWEEN_OPER, operName)
{
}
BetweenAnd::~BetweenAnd()
{
}
void BetweenAnd::optimize()
{
	DBUG_ASSERT(false);
}
void BetweenAnd::release()
{
	if (operator_) {
		operator_->release();
		delete operator_ ;
		operator_ = NULL;
	}
}
const char* BetweenAnd::toString()
{
	return "AND";
}

InOper::InOper (): left_(NULL),right_(NULL), Operator(NODE_TYPE_IN_OPER)
{
}
InOper::InOper(ASTNode* left, ASTNode* right): left_(left),right_(right), Operator(NODE_TYPE_IN_OPER, "IN")
{
}
InOper::~InOper()
{
}
void InOper::optimize()
{
    DBUG_ASSERT(false);
}
void InOper::release()
{
    if (left_) {
        left_->release();
        delete left_;
        left_= NULL;
    }
	
	if (right_) {
		right_ ->release();
		delete right_ ;
		right_ = NULL ;
	}
}
const char* InOper::toString()
{
	if (left_) {
		sqlText_ += left_->toString();
	}
	
	sqlText_ += " IN (" ;
	
	if (right_) {
		sqlText_ += right_ ->toString();
	}	
	
	sqlText_ += ")";
    return sqlText_.c_str();
}

LogicalExpr::LogicalExpr (): left_(NULL), right_(NULL), oper_(NULL), Expr(NODE_TYPE_LOGICAL_EXPR)
{
}
LogicalExpr::LogicalExpr(NodeType type): left_(NULL), right_(NULL), oper_(NULL), Expr(type)
{
}
LogicalExpr::LogicalExpr(ASTNode* left, ASTNode* right, Operator* oper) : left_(left), right_(right), oper_(oper), Expr(NODE_TYPE_LOGICAL_EXPR)
{
}

LogicalExpr::~LogicalExpr()
{
}
void LogicalExpr::release()
{
	if (left_) {
		left_->release();
		delete left_;
		left_ = NULL;
	}
	if(right_){
		right_->release();
		delete right_;
		right_ = NULL;
	}
	if (oper_) {
		oper_->release();
		delete oper_;
		oper_ = NULL;
	}
}
const char* LogicalExpr::toString()
{
    if (left_) {
         sqlText_ += left_->toString() ;
	}
    if (oper_) {
		sqlText_ += " ";
        sqlText_ += oper_->toString();
		sqlText_ += " ";
    }
    if (right_){ //not
        sqlText_ +=  right_->toString();
    }
    sqlText_+= " " ;
	
    return sqlText_.c_str();
}
void LogicalExpr::optimize()
{
}

ArithExpr::ArithExpr() : left_(NULL), right_(NULL), oper_(NULL), Expr(NODE_TYPE_ARITH_EXPR)
{
}
ArithExpr::ArithExpr(NodeType type) : left_(NULL), right_(NULL), oper_(NULL),Expr(NODE_TYPE_ARITH_EXPR)
{
}
ArithExpr::ArithExpr(ASTNode* left, ASTNode* right, Operator* oper) : left_(left), right_(right), oper_(oper), Expr(NODE_TYPE_ARITH_EXPR)
{
}
ArithExpr::~ArithExpr()
{
}
void ArithExpr::optimize()
{
}
const char* ArithExpr::toString()
{
	if (oper_ && oper_->getNodeType() == NODE_TYPE_BETWEEN_OPER) {
		sqlText_ += ((BetweenAnd*)oper_)->getOperator()->toString();
		sqlText_ += " BETWEEN ";
	} 

	if (left_) {
		sqlText_ += left_->toString() ;
		sqlText_ +=" ";
	}
	
	if (oper_) {
		sqlText_ += oper_->toString();
		sqlText_ += " ";
	}
	
	if (right_) {//not 
		sqlText_ +=  right_->toString();
		sqlText_ += " ";
	}
	
	return sqlText_.c_str();
}
void ArithExpr::release()
{
	if (left_){
		left_->release();
		delete left_; 
		left_ = NULL;
	} 
	if (right_) {
		right_ ->release();
		delete right_ ; 
		right_ = NULL;
	}
	if (oper_) {
		oper_->release();
		delete oper_; 
		oper_= NULL;
	}
}
And::And() :left_(NULL), right_(NULL), LogicalExpr(NODE_TYPE_EXPR_AND)
{
}
And::And(ASTNode* left, ASTNode* right): left_ (left), right_(right), LogicalExpr(NODE_TYPE_EXPR_AND)
{
}
And::~And()
{
}

void And::optimize()
{
	DBUG_ASSERT(false);
}

Expr* And::reverse()
{
	ASTNode* tmp = left_ ;
	left_ = right_ ; 
	right_ = tmp ; 
	return this ; 
}
const char* And::toString()
{
	if (left_){
		sqlText_ = left_->toString();
	}
	
	sqlText_ += " AND ";

	if (right_) {
		sqlText_ += right_->toString();
 	}
	return sqlText_.c_str();
}
void And::release()
{
	if (left_) {
		left_->release();
		delete left_ ;
		left_ = NULL;
	}
	
	if (right_) {
		right_ ->release();
		delete right_ ; 
		right_ = NULL; 
	}
}
ASTNode* And::Result()
{
	return NULL;
}
Or::Or() : left_(NULL), right_(NULL), LogicalExpr(NODE_TYPE_EXPR_OR)
{
}
Or::Or(ASTNode* left, ASTNode* right) : left_(left), right_(right), LogicalExpr(NODE_TYPE_EXPR_OR)
{
}
Or::~Or()
{
}
void Or::optimize ()
{
	DBUG_ASSERT(false);
}

Expr* Or::reverse()
{
	ASTNode* tmp = left_; 
	left_ = right_; 
	right_ = tmp ; 

	return this;
}
const char* Or::toString()
{
	if (left_){
		sqlText_ += left_->toString();
	}
	
	sqlText_ += " OR ";

	if (right_) {
		sqlText_ += right_ ->toString(); 
	}
	
	return sqlText_.c_str();
}
void Or::release()
{
	if (left_) {
		left_->release();
		delete left_ ; 
		left_= NULL;
	}
	if (right_) {
		right_ -> release();
		delete right_ ;
		right_ = NULL;
	}
}
ASTNode* Or::Result()
{
	return NULL;
}

ConstValue::ConstValue () : SqlStmt (NODE_TYPE_CONST)
{
}
ConstValue::ConstValue (int val) :  SqlStmt (NODE_TYPE_INT_CONST)
{
	sqlText_ = val;
}
ConstValue::ConstValue(double value) : SqlStmt (NODE_TYPE_FLOAT_CONST)
{
	sqlText_ = value;
}
ConstValue::ConstValue (const char* val) : SqlStmt (NODE_TYPE_STR_CONST)
{
	sqlText_ = val;
}
ConstValue::~ConstValue ()
{
}
void ConstValue::optimize()
{
	DBUG_ASSERT(false);
}
const char* ConstValue::toString()
{
	return "?";
}
void ConstValue::release()
{
}
const char* ConstValue::Text()
{
	return sqlText_.c_str();
}

AlterOpt::AlterOpt () :actType_(ACT_ADD), opt_type_(OPT_EVERY_TYPE),obj_(NULL), SqlStmt(NODE_TYPE_ALTER_TABLE_OPT)
{
}
AlterOpt::AlterOpt (ASTNode* obj) : actType_(ACT_ADD), opt_type_(OPT_EVERY_TYPE), obj_(obj), SqlStmt(NODE_TYPE_ALTER_TABLE_OPT)
{
}
AlterOpt::AlterOpt(ACT_TYPE actType, OPT_TYPE optType, ASTNode* obj) : 
				 actType_(actType), opt_type_(optType), obj_(obj), SqlStmt(NODE_TYPE_ALTER_TABLE_OPT)
{
}
AlterOpt::~AlterOpt()
{
}
void AlterOpt::optimize()
{
}
void AlterOpt::release()
{
	if (obj_){
		obj_->release();
		delete obj_;
		obj_ = NULL;
	}	
}
const char* AlterOpt::toString()
{
	sqlText_= "";
	string objStr("");
	if (actType_ == ACT_ADD) {
		sqlText_ +=" ADD" ;
	} else if (actType_ == ACT_DEL) {
		sqlText_ += " DROP";
	} else if (actType_ == ACT_RENAME) {
		sqlText_ += " RENAME";
	}

	if (opt_type_ == OPT_COLUMN) {
		sqlText_ += " COLUMN" ;
	} else if (opt_type_ == OPT_INDEX) {
		sqlText_ +=" INDEX";
	} else if (opt_type_ == OPT_KEY){
		sqlText_ += " KEY";
	} else if (opt_type_ == OPT_CONSTRAINT) {
		sqlText_ += " CONSTRAINT";
	} else if (opt_type_ == OPT_PRIMARY_KEY) {
		sqlText_ += " PRIMARY KEY";
	} else if (opt_type_ == OPT_FOREIGN_KEY){
		sqlText_ += " FOREIGN KEY";
	}
	
	if (obj_) {
		objStr = obj_->toString();
		
		sqlText_ +=" ";
		sqlText_ += objStr ;
	}
	
	return sqlText_.c_str();
}

AlterOptList::AlterOptList() : options_(NULL), SqlStmt(NODE_TYPE_ALTER_TABLE_OPT_LIST)
{
}
AlterOptList::AlterOptList(AlterOpt* opt) : options_(NULL), SqlStmt(NODE_TYPE_ALTER_TABLE_OPT_LIST)
{
    DBUG_ASSERT (opt->getNodeType() == NODE_TYPE_ALTER_TABLE_OPT);

#if MYSQL_VERSION_ID <= 50700
    LIST* elem = (LIST*)my_malloc (sizeof (LIST), MYF(0));
#else
    LIST* elem = (LIST*)my_malloc (audit_ast_memory_LIST, sizeof (LIST), MYF(0));
#endif 
    elem->data = opt;
    options_ = list_add (options_, elem) ;
}
AlterOptList::~AlterOptList()
{
}

void AlterOptList::optimize()
{
	DBUG_ASSERT(false);
}
void AlterOptList::release()
{
    LIST* start = options_;
    for (;start; start= start->next) { //travel through list. to release all the table objs.
        ASTNode* node = (ASTNode*)start->data;
        DBUG_ASSERT(node->getNodeType() == NODE_TYPE_ALTER_TABLE_OPT);

        node->release();
        delete node;
		node = NULL;
    }

    list_free(options_, 0); //free the list itself.
	
}
const char* AlterOptList::toString()
{
    LIST* start = options_;

    for (;start; start= start->next) { //travel through list. to release all the table objs.
        ASTNode* node = (ASTNode*)start->data;
        DBUG_ASSERT(node->getNodeType() == NODE_TYPE_ALTER_TABLE_OPT);
		
		sqlText_ += node->toString(); 
    }
	
	return sqlText_.c_str();
}
void AlterOptList::mergeOptList(AlterOptList* other)
{//Merge two lists.
	DBUG_ASSERT(false);
}
void AlterOptList::addOpt (AlterOpt* opt)
{
#if MYSQL_VERSION_ID <= 50700
    LIST* elem = (LIST*)my_malloc (sizeof (LIST), MYF(0));
#else
    LIST* elem = (LIST*)my_malloc (audit_ast_memory_LIST, sizeof (LIST), MYF(0));
#endif 
    elem->data = opt;
    options_ = list_add (options_, elem) ;
}

AlterStmt::AlterStmt() :relName_(NULL),options_(NULL),objType_(OBJ_TABLE),SqlStmt (NODE_TYPE_ALTER_STMT, STMT_ALTER_TYPE)
{
}
AlterStmt::AlterStmt (OBJ_TYPE obj_type, ASTNode* relName, AlterOptList* options):
															relName_(relName), options_(options), objType_(obj_type), SqlStmt(NODE_TYPE_ALTER_STMT, STMT_ALTER_TYPE)
{
}
AlterStmt::~AlterStmt() 
{
}
void AlterStmt::optimize()
{
	DBUG_ASSERT(false);
}
const char* AlterStmt::toString()
{
	sqlText_ = "ALTER";
	string optStr(""), objName("");
	if (objType_ == OBJ_TABLE) {
		sqlText_ += " TABLE ";
	}
	else if (objType_ ==OBJ_VIEW){
		sqlText_ +=" VIEW" ;
	}
	else if (objType_ == OBJ_INDEX) {
		sqlText_ += " INDEX";
	}
	else{
		sqlText_ +=" UNKNOWN";
	}
	
	if (relName_){
		objName = relName_->toString();
		sqlText_ += objName; 	
	}
	
	if (options_){
		optStr = options_->toString();
		sqlText_ += optStr; 
	}
	return sqlText_.c_str();
}
void AlterStmt::release()
{
	if (relName_) {
		relName_ ->release();
		delete relName_; 
		relName_ = NULL; 
	}

	if (options_) {
		options_->release(); 
		delete options_; 
		options_ = NULL; 
	}
}

ExprList::ExprList () : exprs_(NULL), Expr (NODE_TYPE_EXPR)
{
}
ExprList::~ExprList()
{
}
void ExprList::optimize ()
{
	DBUG_ASSERT(false);
}
void ExprList::release()
{
    LIST* start = exprs_;
    for (;start; start= start->next) { //travel through list. to release all the table objs.
        Expr* node = (Expr*)start->data;

        node->release();
        delete node;
		node = NULL; 
    }

    list_free(exprs_, 0); //free the list itself.
	
}
const char* ExprList::toString()
{
    LIST* start = exprs_;
    for (;start; start= start->next) { //travel through list. to release all the table objs.
        Expr* node = (Expr*)start->data;
        sqlText_ += node->toString();
    }

    return sqlText_.c_str();
}
void ExprList::addExpr (Expr* expr)
{
	DBUG_ASSERT (expr->getNodeType() == NODE_TYPE_ARITH_EXPR);

#if MYSQL_VERSION_ID < 50700
    LIST* elem = (LIST*)my_malloc (sizeof (LIST), MYF(0));
#else
    LIST* elem = (LIST*)my_malloc (audit_ast_memory_LIST, sizeof (LIST), MYF(0));
#endif 
    elem->data = expr;
    exprs_ = list_add (exprs_, elem) ;
}
void ExprList::mergeList (ExprList* other)
{
	DBUG_ASSERT(false);
}

A_ExprList::A_ExprList () : exprs_(NULL), ASTNode (NODE_TYPE_A_EXPR)
{
}
A_ExprList::~A_ExprList()
{
}
void A_ExprList::optimize ()
{
	DBUG_ASSERT(false);
}
void A_ExprList::release()
{
    LIST* start = exprs_;
    for (;start; start= start->next) { //travel through list. to release all the table objs.
        Expr* node = (Expr*)start->data;
        node->release();
        delete node;
        node = NULL;
    }

    list_free(exprs_, 0); //free the list itself.
}

const char* A_ExprList::toString()
{
    sqlText_="";
	
    LIST* start = exprs_;
    for (;start; start= start->next) { //travel through list. to release all the table objs.
        ASTNode* node = (ASTNode*)start->data;
        sqlText_ += node->toString();
	
		if (start->next){
			sqlText_ += " , ";
		}
    }
	
    return sqlText_.c_str();
}
void A_ExprList::addExpr (ASTNode* expr)
{
    //DBUG_ASSERT (expr->getNodeType() == NODE_TYPE_A_EXPR);
#if MYSQL_VERSION_ID <= 50700
    LIST* elem = (LIST*)my_malloc (sizeof (LIST), MYF(0));
#else
    LIST* elem = (LIST*)my_malloc (audit_ast_memory_LIST, sizeof (LIST), MYF(0));
#endif 
    elem->data = expr;
    exprs_ = list_add (exprs_, elem) ;
}
void A_ExprList::mergeList (A_ExprList* other)
{
    DBUG_ASSERT(false);
}

UpdateStmt::UpdateStmt(): orig_(NULL), expr_(NULL), where_ (NULL), SqlStmt(NODE_TYPE_UPDATE_STMT,STMT_UPDATE_TYPE)
{
}
UpdateStmt::~UpdateStmt()
{
}

void UpdateStmt::optimize()
{
	DBUG_ASSERT(false);
}
void UpdateStmt::release()
{
	if (orig_) {
		orig_->release();
		delete orig_ ; 
		orig_ = NULL;
	}
	if (expr_) {
		expr_->release();
		delete expr_ ; 
		expr_ = NULL;
	}
	if (from_) {
		from_->release();
		delete from_; 
		from_ = NULL; 
	}
	if (where_) {
		where_ ->release(); 
		delete where_ ; 
		where_ = NULL ;
	}
}
const char* UpdateStmt::toString()
{
	sqlText_ = "UPDATE";
	if (orig_) {
		sqlText_ +=" ";
		sqlText_ += orig_->toString();
	}
	
	if (expr_) {
		sqlText_ +=" SET ";
		sqlText_+= expr_->toString();
	}
	if (from_) {
		sqlText_ += from_->toString();
	}
	if (where_) {
		sqlText_ += where_->toString(); 
	}
	
	return sqlText_.c_str(); 
}

DeleteStmt::DeleteStmt(): orig_(NULL), where_(NULL), SqlStmt (NODE_TYPE_DELETE_STMT,STMT_DEL_TYPE)
{
}
DeleteStmt::~DeleteStmt () 
{
}
void DeleteStmt::optimize ()
{
	DBUG_ASSERT(false);
}
void DeleteStmt::release()
{
	if (orig_) {
		orig_ ->release();
		delete orig_;
		orig_ = NULL;
	}
	if (where_) {
		where_ ->release();
		delete where_; 
		where_=NULL;
	}
}

const char* DeleteStmt::toString()
{
	sqlText_ = "DELETE FROM" ;
	if (orig_){
		sqlText_ += " ";
		sqlText_ += orig_->toString();
	}
	
	if (where_) {
		sqlText_ += where_->toString();
	}
	return sqlText_.c_str();
}

InsertStmt::InsertStmt(): cols_(NULL), values_(NULL), SqlStmt (NODE_TYPE_INSERT_STMT, STMT_INSERT_TYPE)
{
}
InsertStmt::~InsertStmt()
{
}
void InsertStmt::optimize()
{
	DBUG_ASSERT(false);
}
void InsertStmt::release()
{
	if (cols_) {
		cols_->release();
		delete cols_; 
		cols_ = NULL;
	}
	if (values_) {
		values_->release(); 
		delete values_ ; 
		values_ = NULL; 
	}
}
const char* InsertStmt::toString()
{
	sqlText_ = ("INSERT INTO"); 
	if (cols_) {
		sqlText_ +=" ";
		sqlText_ += cols_->toString(); 
	}
	
	if (values_){
		sqlText_ +=" VALUES";
		sqlText_ += values_->toString();
	}
	
	return sqlText_.c_str();
}

DropStmt::DropStmt () : act_type_(ACT_NONE), obj_(NULL), SqlStmt(NODE_TYPE_DROP_STMT, STMT_DROP_TYPE)
{
}
DropStmt::DropStmt(ACT_TYPE type , ASTNode* obj) : act_type_(type), obj_(obj), SqlStmt(NODE_TYPE_DROP_STMT, STMT_DROP_TYPE)
{
}
DropStmt::~DropStmt()
{
}
void DropStmt::optimize()
{
	DBUG_ASSERT(false);
}
void DropStmt::release()
{
	if (obj_) {
		obj_->release();
		delete obj_; 
		obj_ = NULL;
	}
}
const char*  DropStmt::toString()
{
	sqlText_ = ("DROP ");
	string typeStr, objStr;
	switch (act_type_) {
		case ACT_NONE: break; 

		case ACT_DATABASE: {
			typeStr+="DATABASE" ;
		} break; 
		case ACT_USER:{
			typeStr +="USER" ;	
		}break; 
		case ACT_ROLE : {
			typeStr +="ROLE";
		}break; 
		case ACT_INDEX: {
			typeStr +="INDEX";
		}break; 
		default :{
			typeStr="";
		}
			break;
	}

	if (typeStr.length()) {	
		sqlText_ += typeStr; 
		sqlText_ +=" ";
	}
	if (obj_) {
		objStr = obj_->toString();
		sqlText_ += objStr; 
	}
	
	return sqlText_.c_str();
}

CreateRuleStmt::CreateRuleStmt(ASTNode* sql, ASTNode* pattern, ASTNode* sqlAction) : sql_(sql), pattern_(pattern), sqlAction_(sqlAction),
																						SqlStmt(NODE_TYPE_CREATERULE_STMT)
{
}
CreateRuleStmt::~CreateRuleStmt ()
{
}
    
void CreateRuleStmt::optimize ()
{

}
void CreateRuleStmt::release ()
{
	if (sql_) {
		sql_->release();
		delete sql_; 
		sql_ = NULL;
	}
	if (pattern_) {
		pattern_ ->release();
		delete pattern_ ; 
		pattern_ = NULL;
	}
	if (sqlAction_) {
		sqlAction_->release(); 
		delete sqlAction_; 
		sqlAction_ = NULL; 
	}
}
const char* CreateRuleStmt::toString ()
{
	DBUG_ASSERT (false) ;
	return NULL; 
}

const char* CreateRuleStmt::sqlText () 
{
	if (sql_) 
		return sql_->Text();
	else return NULL;
}      
const char* CreateRuleStmt::sqlPattern()
{
	if (pattern_)
		return pattern_->toString();
	else return NULL; 
}
const char* CreateRuleStmt::sqlAction()
{
	if (sqlAction_)
		return sqlAction_ -> Text();
	else return NULL;
}

int CreateRuleStmt::execute ()
{
	return 0;
}

UpdateRuleStmt::UpdateRuleStmt(ASTNode* sql, ASTNode* pattern, ASTNode* action) : sql_(sql), pattern_(pattern), action_(action),
											SqlStmt(NODE_TYPE_UPDATERULE_STMT) 
{

}

UpdateRuleStmt::~UpdateRuleStmt()
{

}

void UpdateRuleStmt::optimize()
{

}
void UpdateRuleStmt::release()
{
	if (sql_) {
		sql_->release();
		delete sql_ ; 
		sql_ = NULL; 
	}

	if (pattern_) {
		pattern_ ->release();
		delete pattern_ ;
		pattern_ = NULL;
	}
	if (action_) {
		action_->release();
		delete action_ ;
		action_ = NULL;
	}
}

const char* UpdateRuleStmt::toString()
{
	DBUG_ASSERT (false);
	return NULL;
}

int UpdateRuleStmt::execute ()
{
	return 0;
}
const char* UpdateRuleStmt::sqlText()
{
	if (sql_)
		return sql_->Text();
	return NULL;
}
const char* UpdateRuleStmt::sqlPattern ()
{
	if (pattern_) 
		return pattern_-> toString();
	return NULL;
}
const char* UpdateRuleStmt::sqlAction()
{
	if (action_)
		return action_->Text();
	return NULL;
}

DropRuleStmt::DropRuleStmt(ASTNode* sql) : sql_(sql), SqlStmt (NODE_TYPE_DROPRULE_STMT)
{

}

DropRuleStmt::~DropRuleStmt ()
{

}

void DropRuleStmt::optimize ()
{

}

void DropRuleStmt::release()
{
	if (sql_) {
		sql_->release();
		delete sql_; 
		sql_ = NULL;
	}
}
const char* DropRuleStmt::toString()
{
	return NULL;
}
int DropRuleStmt::execute()
{
	return 0;
}

const char* DropRuleStmt::sqlText ()
{
	if (sql_)
		return sql_->Text();
	return NULL; 
}
