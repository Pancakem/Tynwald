#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gen.h"
#include "mpc.h"

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

char *getvar(int count)
{
	int len = ndigits(count) + 2;
	char *val = malloc(sizeof(char) * len);
	snprintf(val, len, "t%d", count);
	return val;
}

char *term_rval(mpc_ast_t *exp, int *counter)
{
	return "term";
}

char *lexp_rval(mpc_ast_t *exp, int *counter)
{
	if (exp->children_num == 0) {
		fprintf(stderr, "0 child lexp\n");
		exit(EXIT_FAILURE);
	}

	char *first_rval = term_rval(exp->children[0], counter);
	if (exp->children_num == 1) {
		return first_rval;
	}

	if (exp->children_num < 3) {
		fprintf(stderr, "%d child lexp\n", exp->children_num);
		exit(EXIT_FAILURE);
	}

	mpc_ast_t *op = exp->children[1];
	if (strcmp(op->tag, "char") != 0) {
		fprintf(stderr, "non char op '%s'\n", op->tag);
		exit(EXIT_FAILURE);
	}
	
	/* need recursion in grammar */

	char *temp = getvar(*counter);
	printf("%s := %s %s %s\n", temp, first_rval, op->contents, "<place>");
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
			printf("ret %s\n", lexp_rval(stmt->children[1], counter));
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
	mpc_ast_t *main = findmain((mpc_ast_t *) res->output);
	if (!main) {
		fprintf(stderr, "cannot find main\n");
		exit(EXIT_FAILURE);
	}
	proc_gen(main);
}
