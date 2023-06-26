#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "gen.h"
#include "mpc.h"

static char *new_str(char *input)
{
	int len = strlen(input) + 1;
	char *s = malloc(sizeof(char) * len);
	snprintf(s, len, "%s", input);
	return s;
}

static mpc_ast_t *firstchild(mpc_ast_t *ast)
{
	char *barloc = strchr(ast->tag, '|'),
	     *gtloc = strchr(ast->tag, '>');
	if (barloc && (!gtloc || barloc + 1 < gtloc)) {
		mpc_ast_t *new = mpc_ast_new(barloc+1, ast->contents);
		new->children_num = ast->children_num;
		new->children = ast->children;
		return new;
	}
	return (ast->children_num > 0) ? ast->children[0] : NULL;
}

static char *proc_getident(mpc_ast_t *ast)
{
	/* NB: currently procedure <ident> is 2nd child in grammar */
	if (ast->children_num < 2) {
		fprintf(stderr, "invalid procedure children_num: %d\n",
			ast->children_num);
		exit(EXIT_FAILURE);
	}
	/* <ident> */
	return ast->children[1]->contents;
}

static mpc_ast_t *findmain(mpc_ast_t *ast)
{
	for (int i = 0; i < ast->children_num; i++) {
		mpc_ast_t *c = ast->children[i];
		if (strcmp(c->tag, "procedure|>") == 0 && 
				strcmp(proc_getident(ast), "main")) {
			return c;
		}
	}
	return NULL;
}

static mpc_ast_t *findbody(mpc_ast_t *ast)
{
	for (int i = 0; i < ast->children_num; i++) {
		mpc_ast_t *c = ast->children[i];
		if (strcmp(c->tag, "body|>") == 0) {
			return c;
		}
	}
	return NULL;
}

int
ndigits(int n)
{
	int i = 1;
	while (n > 9) {
		n /= 10;
		i++;
	}
	return i;
}

char *getvar(int *counter)
{
	int len = ndigits(*counter) + 2;
	char *val = malloc(sizeof(char) * len);
	snprintf(val, len, "t%d", (*counter)++);
	return val;
}

char *lexp_getrval(mpc_ast_t *, int *counter);

char *factor_getrval(mpc_ast_t *factor, int *counter)
{
	/* 
	 * XXX: only catering for first two productions here
	 *
	 * factor	: '(' <lexp> ')'
	 * 		| <number> ;
	 */

	mpc_ast_t *first = firstchild(factor);
	if (factor->children_num == 0) {
		/* XXX: check more rigorously that first is a number */
		return new_str(factor->contents);
	}

	/* assure bracketed expressions */
	assert(factor->children_num == 3);
	mpc_ast_t *lbrac = factor->children[0],
		*lexp = factor->children[1],
		*rbrac = factor->children[2];
	assert(strcmp(lbrac->tag, "char") == 0
		&& strcmp(rbrac->tag, "char") == 0);

	return lexp_getrval(lexp, counter);
}

char *term_getrval(mpc_ast_t *term, int *counter)
{
	/*
	 * term		: <factor> ('*' | '/' | '%') <term>
	 * 		| <factor> ;
	 */
	assert(term->children_num == 0 || term->children_num == 3);

	/* XXX: need to dispose of factor in some cases */
	mpc_ast_t *factor = firstchild(term);
	assert(factor);

	char *factor_rval = factor_getrval(factor, counter);
	if (term->children_num == 0) {
		return factor_rval;
	}

	assert(term->children_num == 3);

	mpc_ast_t *op = term->children[1];
	mpc_ast_print(term);
	assert(strcmp(op->tag, "char") == 0);
	
	char *term_rval = term_getrval(term->children[2], counter);

	char *temp = getvar(counter);
	printf("%s := %s %s %s\n", temp, factor_rval, op->contents, term_rval);

	free(factor_rval);
	free(term_rval);

	return temp;
}

char *lexp_getrval(mpc_ast_t *exp, int *counter)
{
	/*
	 * lexp		: <term> ('+' | '-') <lexp>
	 * 		| <term> ;
	 */
	assert(exp->children_num == 0 || exp->children_num == 3);
	
	/* XXX: need to dispose of term in some cases */
	mpc_ast_t *term = firstchild(exp);
	assert(term);

	char *term_rval = term_getrval(term, counter);
	if (exp->children_num == 0) {
		return term_rval;
	}

	assert(exp->children_num == 3);

	mpc_ast_t *op = exp->children[1];
	assert(strcmp(op->tag, "char") == 0);
	
	char *lexp_rval = lexp_getrval(exp->children[2], counter);

	char *temp = getvar(counter);
	printf("%s := %s %s %s\n", temp, term_rval, op->contents, lexp_rval);

	free(term_rval);
	free(lexp_rval);

	return temp;
}

void stmt_gen(mpc_ast_t *stmt, int *counter)
{
	if (stmt->children_num == 0) {
		fprintf(stderr, "0 child statement\n");
		exit(EXIT_FAILURE);
	}
	char *firstcontents = stmt->children[0]->contents;
	if (strcmp(firstcontents, "return") == 0) {
		/* ends with semicolon, so if there are more than 2 children in
		 * the statement then there is at least one <lexp>. if there are
		 * more than 3 children there is some kind of error. */
		switch (stmt->children_num) {
		case 2:
			printf("ret\n");
			return;
		case 3:
			printf("ret %s\n", lexp_getrval(stmt->children[1],
				counter));
			return;
		default:
			fprintf(stderr, "%d child statement\n", stmt->children_num);
			exit(EXIT_FAILURE);

		}
	}
}

void proc_gen(mpc_ast_t *proc)
{
	/* focusing on body alone currently and not worrying about arguments and
	 * return type */
	mpc_ast_t *body = findbody(proc);
	if (!body) {
		fprintf(stderr, "empty body\n");
		exit(EXIT_FAILURE);
	}
	/* ignore decls and focus on statements */
	int counter = 0;
	for (int i = 0; i < body->children_num; i++) {
		mpc_ast_t *c = body->children[i];
		if (strcmp(c->tag, "stmt|>") != 0) {
			continue;
		}
		stmt_gen(c, &counter);
	}
}

void gen(mpc_result_t *res)
{
	mpc_ast_print((mpc_ast_t *) res->output);
	mpc_ast_t *main = findmain((mpc_ast_t *) res->output);
	if (!main) {
		fprintf(stderr, "cannot find main\n");
		exit(EXIT_FAILURE);
	}
	proc_gen(main);
}
