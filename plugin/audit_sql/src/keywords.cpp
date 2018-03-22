/**
* the keywords definition.
* path: query/parser/keywords/keywords.cpp
*/

#include <string.h>

#include <keywords.h>
#include <parserGramm.h>

scan_keyword_t KeyWord::scanKeyWordsList_[] ={
	TRANS_KEYWORD("add",ADD_P,RESERVED_KEYWORD), //1
	TRANS_KEYWORD("alter",ALTER,RESERVED_KEYWORD),
	TRANS_KEYWORD("and",AND,RESERVED_KEYWORD),
	TRANS_KEYWORD("as",AS,RESERVED_KEYWORD),
	TRANS_KEYWORD("begin",BEGIN_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("between",BETWEEN,RESERVED_KEYWORD),
	TRANS_KEYWORD("by",BY,RESERVED_KEYWORD),
	TRANS_KEYWORD("column",COLUMN,RESERVED_KEYWORD),
	TRANS_KEYWORD("commit",COMMIT_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("constraint",CONSTRAINT,RESERVED_KEYWORD),
	TRANS_KEYWORD("create",CREATE,RESERVED_KEYWORD),
	TRANS_KEYWORD("current",CURRENT_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("database",DATABASE,RESERVED_KEYWORD),
	TRANS_KEYWORD("delete",DELETE,RESERVED_KEYWORD),
	TRANS_KEYWORD("distinct",DISTINCT,RESERVED_KEYWORD),
	TRANS_KEYWORD("drop",DROP,RESERVED_KEYWORD),
	TRANS_KEYWORD("foreign",FOREIGN,RESERVED_KEYWORD),
	TRANS_KEYWORD("from",FROM,RESERVED_KEYWORD),
	TRANS_KEYWORD("group",GROUP_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("having",HAVING,RESERVED_KEYWORD),
	TRANS_KEYWORD("ignore",IGNORE,RESERVED_KEYWORD),
	TRANS_KEYWORD("in",IN_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("insert",INSERT,RESERVED_KEYWORD),
	TRANS_KEYWORD("into",INTO,RESERVED_KEYWORD),
	TRANS_KEYWORD("key",KEY_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("killsession",KILLSESSION,RESERVED_KEYWORD),
	TRANS_KEYWORD("like",LIKE,RESERVED_KEYWORD),
	TRANS_KEYWORD("limit",LIMIT,RESERVED_KEYWORD),
	TRANS_KEYWORD("not",NOT,RESERVED_KEYWORD),
	TRANS_KEYWORD("of",OF_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("or",OR,RESERVED_KEYWORD),
	TRANS_KEYWORD("order",ORDER_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("primary",PRIMARY,RESERVED_KEYWORD),
	TRANS_KEYWORD("references",REFERENCES,RESERVED_KEYWORD),
	TRANS_KEYWORD("role",ROLE,RESERVED_KEYWORD),
	TRANS_KEYWORD("rollback",ROLLBACK_P,RESERVED_KEYWORD),
	TRANS_KEYWORD("rule",RULE,RESERVED_KEYWORD),
	TRANS_KEYWORD("select",SELECT,RESERVED_KEYWORD),
	TRANS_KEYWORD("set",SET,RESERVED_KEYWORD),
	TRANS_KEYWORD("table",TABLE,RESERVED_KEYWORD),
	TRANS_KEYWORD("update",UPDATE,RESERVED_KEYWORD),
	TRANS_KEYWORD("user",USER,RESERVED_KEYWORD),
	TRANS_KEYWORD("values",VALUES,RESERVED_KEYWORD),
	TRANS_KEYWORD("where",WHERE,RESERVED_KEYWORD),
	TRANS_KEYWORD("with",WITH,RESERVED_KEYWORD)//38
};

ScanKeyWord
KeyWord::KeyWordLookup (const char* keyword, const ScanKeyWord keywords_list, int number_keywords )
{
	char		word[64];
	ScanKeyWord low = NULL;
	ScanKeyWord high = NULL;

	int len = strlen(keyword);
	if (len >= 64)
		return NULL;

	/*
	 * Apply an ASCII-only downcasing.  We must not use tolower() since it may
	 * produce the wrong translation in some locales (eg, Turkish).
	 */
	for (int i = 0; i < len; i++)
	{
		char		ch = keyword[i];

		if (ch >= 'A' && ch <= 'Z')
			ch += 'a' - 'A';
		word[i] = ch;
	}
	word[len] = '\0';

	/*
	 * Now do a binary search using plain strcmp() comparison.
	 */
	low = keywords_list;
	high = keywords_list + (number_keywords - 1);
	while (low <= high)
	{
		ScanKeyWord middle ;
		int			difference;

		middle = low + (high - low) / 2;
		if (!middle->key_name_){
			high = middle - 1;
			continue; 
		}
		difference = strcmp(middle->key_name_, word);
		if (difference == 0)
			return middle;
		else if (difference < 0)
			low = middle + 1;
		else
			high = middle - 1;
	}

	return NULL;
}

