#ifndef PARSER_H
#define PARSER_H

#include  "mpc.h"

#define GRAMMAR_FILE "grammar"

struct parser {
    mpc_parser_t *Ident;
    mpc_parser_t *Number;
    mpc_parser_t *Character;
    mpc_parser_t *String;
    mpc_parser_t *Factor;
    mpc_parser_t *Term;
    mpc_parser_t *Lexp;
    mpc_parser_t *Stmt;
    mpc_parser_t *Exp;
    mpc_parser_t *Typeident;
    mpc_parser_t *Decls;
    mpc_parser_t *Args;
    mpc_parser_t *Body;
    mpc_parser_t *Procedure;
    mpc_parser_t *Program;
} parser_t;

void init_parser(struct parser*);
void delete_parser(struct parser*psr);
mpc_result_t *parse_grammar(struct parser *psr, const char *);

#endif
