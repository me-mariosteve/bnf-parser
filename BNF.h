#ifndef BNF_H_INCLUDED
#define BNF_H_INCLUDED


#include <stddef.h>
#include <stdbool.h>


typedef struct BNF_Context {
	const char *str;
	const size_t len;
	size_t i;
} BNF_Context;

typedef struct BNF_Grammar BNF_Grammar;
typedef struct BNF_Rule BNF_Rule;
typedef struct BNF_Expr BNF_Expr;
typedef struct BNF_Symbol BNF_Symbol;


struct BNF_Grammar {
	BNF_Rule *rules;
	size_t size;
};

struct BNF_Rule {
	char *name;
	BNF_Expr *expr_l;
	size_t size;
};

struct BNF_Expr {
	BNF_Symbol *sym_l;
	size_t size;
};

struct BNF_Symbol {
	char *sym;
	size_t size;
	bool is_term;
};


bool BNF_IsIdentChar(const char c);

#define BNF_SKIP_WHITESPACE	1
#define BNF_SKIP_NEWLINE	2
#define BNF_SKIP_COMMENT	3
#define BNF_SKIP_ALL		( BNF_SKIP_WHITESPACE | BNF_SKIP_NEWLINE | BNF_SKIP_COMMENT )
void BNF_SkipIgnored(BNF_Context *ctx, char flags);

int BNF_ParseGrammar(BNF_Grammar *const dst, BNF_Context *ctx);
int BNF_ParseRule(BNF_Rule *const dst, BNF_Context *ctx);
int BNF_ParseExpr(BNF_Expr *const dst, BNF_Context *ctx);
int BNF_ParseSymbol(BNF_Symbol *const dst, BNF_Context *ctx);
int BNF_ParseTerm(BNF_Symbol *const dst, BNF_Context *ctx);
int BNF_ParseNonTerm(BNF_Symbol *const dst, BNF_Context *ctx);

#endif /* BNF_H_INCLUDED */

