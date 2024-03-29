
#include "ast.h"

token_t *_ast_collect_funcarg(token_t *comma, token_t *token);
void _ast_print(token_t *token, int depth);

/**
 * @brief Initialize a token to be an AST node. Return the node given to it
 *
 * @param token
 * @return token_t*
 */
token_t *ast_make_node(token_t *token)
{
  token->child = token->sibling = token->parent = NULL;
  return token;
}

int ast_isleaf(token_t *token) { return token->child == NULL; }

/**
 * @brief Update the offset using the first non-NULL token in child list
 *
 * @param token
 */
void ast_update_offset(token_t *token)
{
  if (token->offset)
    return;
  token_t *child = token->child;
  while (child && !child->offset)
    child = child->sibling;
  if (child)
    token->offset = child->offset;
}

token_t *ast_append_child(token_t *token, token_t *child)
{
  if (token->child == NULL)
  {
    token->child = child;
  }
  else
  {
    token_t *last = token->child;
    while (last->sibling != NULL)
      last = last->sibling;
    last->sibling = child;
  }
  child->sibling = NULL;
  child->parent = token;
  ast_update_offset(token);
  return token;
}

/**
 * @brief Adds the node as the first child of the token
 *
 * @param token
 * @param child
 * @return token_t*
 */
token_t *ast_push_child(token_t *token, token_t *child)
{
  child->sibling = token->child;
  token->child = child;
  child->parent = token;
  ast_update_offset(token);
  return token;
}

/**
 * @brief Adds a node as a sibling after the given one, adding a child
 *
 * @param token
 * @param child
 * @return token_t*
 */
token_t *ast_insert_after(token_t *token, token_t *child)
{
  child->sibling = token->sibling;
  token->sibling = child;
  child->parent = token->parent;
  ast_update_offset(token);
  return token;
}

/**
 * @brief Remove from parent node. Assume there is a parent node. Returns the node itself
 *
 * @param token
 * @return token_t*
 */
token_t *ast_remove(token_t *token)
{
  token_t *parent = token->parent;
  if (parent->child == token)
    parent->child = token->sibling;
  else
  {
    token_t *curr = parent->child; // Assumes that the tree is correctly formed, so curr will not be NULL
    while (curr->sibling != token)
      curr = curr->sibling;
    curr->sibling = token->sibling;
  }
  return token;
}

void ast_print(token_t *token) { _ast_print(token, 0); }

void _ast_print(token_t *token, int depth)
{
  for (int i = 0; i < depth * 2; i++)
    if (i % 2 == 0)
      printf("|");
    else
      printf(" ");
  const char *symstr = token_symstr(token->type);
  printf("%04d:%04d:%s %s\n",
         token->type,
         token->offset ? error_get_offset(token->offset) : 0,
         token_typestr(token->type),
         token->type == T_BASETYPE ? token_decl_print(token->decl_prop) : (symstr == NULL ? (token->type >= T_LITERALS_BEGIN && token->type < T_LITERALS_END ? token->str : "") : symstr));
  for (token_t *child = token->child; child != NULL; child = child->sibling)
    _ast_print(child, depth + 1);
}

void ast_destroy(token_t *token)
{
  while (token->child != NULL)
  {
    token_t *next = token->child->sibling;
    ast_destroy(token->child);
    token->child = next;
  }
  token_destroy(token);
}

int ast_child_count(token_t *token)
{
  int count = 0;
  token_t *child = token->child;
  while (child)
  {
    count++;
    child = child->sibling;
  }
  return count;
}

/**
 * @brief Get n-th child; Return NULL if index is larger than the number of children
 * 
 * @param token 
 * @param index 
 * @return token_t* 
 */
token_t *ast_getchild(token_t *token, int index)
{
  assert(index >= 0 && token != NULL);
  token = token->child;
  while (token != NULL && index-- != 0)
    token = token->sibling;
  return token;
}

/**
 * @brief Returns the last inserted node
 * 
 * @param comma 
 * @param token 
 * @return token_t* 
 */
token_t *_ast_collect_funcarg(token_t *comma, token_t *token)
{
  assert(ast_getchild(comma, 0) != NULL && ast_getchild(comma, 1) != NULL);
  token_t *child1 = comma->child, *child2 = child1->sibling;
  if (child1->type != EXP_COMMA)
  {
    ast_insert_after(token, child2);
    ast_insert_after(token, child1);
    token = child1;
  }
  else
  {
    ast_insert_after(token, child2);
    token = _ast_collect_funcarg(child1, token);
  }
  token_destroy(comma);
  return token;
}

/**
 * @brief Transforms function argument from comma expression to flat structure
          Three cases: argument-less func; one argument func (must not be comma exp)
          and functions with >= 2 arguments
 * 
 * @param token 
 */
void ast_collect_funcarg(token_t *token)
{
  assert(token->type == EXP_FUNC_CALL);
  token_t *comma = ast_getchild(token, 1);
  if (comma == NULL || comma->type != EXP_COMMA)
    return;
  // The comma node has been freed. The function returns the last node inserted
  token->child->sibling = _ast_collect_funcarg(comma, comma);
}

/**
 * @brief Transforms conditional expression from two 2-operand operators to
          a single cond operator
 * 
 * @param token 
 */
void ast_movecond(token_t *token)
{
  assert(token->type == EXP_COND);
  if (ast_getchild(token, 1)->type != EXP_COLON)
    error_row_col_exit(token->offset, "Operator \'?\' must be followed by operator \':\'\n");
  token_t *colon = ast_getchild(token, 1), *child2 = ast_getchild(colon, 1);
  ast_append_child(token, colon->child);
  ast_append_child(token, child2);
  token->child->sibling = colon->child;
  token_destroy(colon);
}

/**
 * @brief Returns a pointer to the first child of given type, or NULL
 * 
 * @param token 
 * @param type 
 * @return token_t* 
 */
token_t *ast_gettype(token_t *token, token_type_t type)
{
  for (token = token->child; token && token->type != type; token = token->sibling)
    ;
  return token;
}
