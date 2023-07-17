
#ifndef PARSE_COMP_H
#define PARSE_COMP_H

#include "parse_decl.h"

typedef parse_exp_cxt_t parse_comp_cxt_t;

parse_decl_cxt_t *parse_comp_new(char *input);
void parse_comp_destroy(parse_decl_cxt_t *cxt);
token_t *parse_comp(parse_exp_cxt_t *cxt);
int parse_name_body(parse_comp_cxt_t *cxt, token_t *root);
token_t *parse_struct_union(parse_comp_cxt_t *cxt, token_t *root);
token_t *parse_enum(parse_comp_cxt_t *cxt, token_t *root);

#endif