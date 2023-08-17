
#include "parse_exp.h"
#include "parse_decl.h"
#include "parse_comp.h"
#include "parse_stmt.h"

#ifndef PARSE_H
#define PARSE_H

typedef parse_exp_cxt_t parse_cxt_t;

parse_cxt_t *parse_new(char *input);
void parse_destroy(parse_cxt_t *cxt);
token_t *parse(parse_cxt_t *cxt);

#endif