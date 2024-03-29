
#ifndef PARSE_DECL_H
#define PARSE_DECL_H

#include "parse_exp.h"
#include "hashtable.h"

#define PARSE_DECL_NOBASETYPE  0
#define PARSE_DECL_HASBASETYPE 1
typedef parse_exp_cxt_t parse_decl_cxt_t;

parse_decl_cxt_t *parse_decl_new(char *input);
void parse_decl_destroy(parse_decl_cxt_t *cxt);
int parse_decl_isbasetype(parse_decl_cxt_t *cxt, token_t *token);
token_t *parse_decl_next_token(parse_decl_cxt_t *cxt);
void parse_typespec(parse_decl_cxt_t *cxt, token_t *basetype);
token_t *parse_decl_basetype(parse_decl_cxt_t *cxt);
token_t *parse_decl(parse_decl_cxt_t *cxt, int hasbasetype);

#endif