
#ifndef AST_H
#define AST_H

#include "token.h"

token_t *ast_make_node(token_t *token);
int ast_isleaf(token_t *token);
void ast_update_offset(token_t *token);
token_t *ast_append_child(token_t *token, token_t *child);
token_t *ast_push_child(token_t *token, token_t *child);
token_t *ast_insert_after(token_t *token, token_t *child);
token_t *ast_remove(token_t *token);
void ast_print(token_t *token);
void _ast_print(token_t *token, int depth);
void ast_destroy(token_t *token);
int ast_child_count(token_t *token);
token_t *ast_getchild(token_t *token, int index);
void ast_collect_funcarg(token_t *token);
void ast_movecond(token_t *token);
token_t *ast_gettype(token_t *token, token_type_t type);

#endif