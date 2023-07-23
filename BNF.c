#include "BNF.h"

#include <stdlib.h>	// exit
#include <stdio.h>	// fprintf, perror

#include "utils.h"	// xmalloc, xrealloc

#define BNF_TRACE(fmt, ...)	fprintf(stderr, "%s:%i:%s: " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); 

#define _BNF_check_not_NULL(ptr)			\
	if (ptr == NULL) {				\
		BNF_TRACE("ERROR: %s == NULL", #ptr);	\
		exit(EXIT_FAILURE);			\
	}


bool BNF_IsIdentChar(const char c) {
	return (
			(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') || c == '-'
	       ) ? true : false;
}

void BNF_Skip(BNF_Context *c, char flags) {
	while (c->i < c->len) {
		if (BNF_SKIP_WHITESPACE & flags && ( c->str[c->i] == ' ' || c->str[c->i] == '\t' ) ) {
			c->i++;
		} else if (BNF_SKIP_NEWLINE & flags && c->str[c->i] == '\n') {
			c->i++;
		} else if (BNF_SKIP_COMMENT & flags && c->str[c->i] == '#') {
			while (c->i < c->len && c->str[c->i] != '\n') {
				c->i++;
			}
			// Skip the line only with BNF_SKIP_NEWLINE, as it may
			// break stuff like find everything on this line, while
			// ignoring comments.
		} else {
			break;
		}
	}
}


int BNF_ParseGrammar(BNF_Grammar *const dst, BNF_Context *c) {
	_BNF_check_not_NULL(dst);
	dst->rules = xmalloc(0);
	dst->size = 0;
	while (c->i < c->len) {
		dst->size++;
		dst->rules = xreallocarray(dst->rules, dst->size, sizeof(BNF_Rule));
		// BNF_ParseRule skips BNF_SKIP_* after success
		if (!BNF_ParseRule(&dst->rules[dst->size-1], c)) {
			dst->size--;
			break;
		}
	}
	dst->rules = xreallocarray(dst->rules, dst->size, sizeof(BNF_Rule));
	if (c->i < c->len-1) {
		// We didn't get to the end of the string so we failed
		return 0;
	}
	return 1;
}

int BNF_ParseRule(BNF_Rule *const dst, BNF_Context *c) {
	_BNF_check_not_NULL(dst);
	BNF_Skip(c, BNF_SKIP_ALL);
	BNF_Symbol s;
	if (!BNF_ParseSymbol(&s, c) || s.is_term != false) {
		return 0;
	}
	dst->name = s.sym;
	BNF_Skip(c, BNF_SKIP_ALL);
	if (
		c->i > c->len-3 || ! (
			c->str[c->i] == c->str[c->i+1] && c->str[c->i] == ':'
			&& c->str[c->i+2] == '='
	) ) {
		return 0;
	}
	dst->size = 0;
	return 1;
}

int BNF_ParseExpr(BNF_Expr *const dst, BNF_Context *c) {
	_BNF_check_not_NULL(dst);
	while (c->i < c->len) {
		BNF_Skip(c, BNF_SKIP_WHITESPACE|BNF_SKIP_COMMENT);
		dst->size++;
		dst->sym_l = xreallocarray(dst->sym_l, dst->size, sizeof(BNF_Symbol));
		if (!BNF_ParseSymbol(&dst->sym_l[dst->size-1], c)) {
			break;
		}
	}
	return 1;
}

int BNF_ParseSymbol(BNF_Symbol *const dst, BNF_Context *c) {
	size_t i_bak = c->i;
	if (BNF_ParseNonTerm(dst, c)) {
		return 1;
	}
	free(dst->sym);
	c->i = i_bak;
	if (BNF_ParseNonTerm(dst, c)) {
		return 1;
	}
	return 0;
}

int BNF_ParseTerm(BNF_Symbol *const dst, BNF_Context *c) {
	_BNF_check_not_NULL(dst);
	if (c->len < 2 || c->str[0] != '"') {
		return 0;
	}
	dst->is_term = true;
	dst->sym = xmalloc(0);
	dst->size = 0;
	while (c->i < c->len && c->str[c->i] != '"') {
		char next;
		if (c->str[c->i] == '\\') {
			c->i++;
			if (c->i == c->len) {
				return 0;
			}
			if (c->str[c->i] == '"' || c->str[c->i] == '\\') {
				next = c->str[c->i];
			} else {
				return 0;
			}
		} else {
			next = c->str[c->i];
		}
		dst->size++;
		dst->sym = xreallocarray(dst->sym, dst->size, sizeof(char));
		dst->sym[dst->size-1] = next;
	}
	return 1;
}


int BNF_ParseNonTerm(BNF_Symbol *const dst, BNF_Context *c) {
	_BNF_check_not_NULL(dst);
	if (c->len < 2 || c->str[c->i] != '<') {
		return 0;
	}
	dst->is_term = false;
	dst->sym = xmalloc(0);
	dst->size = 0;
	while (c->i < c->len && BNF_IsIdentChar(c->str[c->i])) {
		dst->size++;
		dst->sym = xreallocarray(dst->sym, dst->size, sizeof(char));
		dst->sym[dst->size-1] = c->str[c->i];
		c->i++;
	}
	if (dst->size == 0 || c->str[c->i] != '>') {
		return 0;
	}
	return 1;
}
