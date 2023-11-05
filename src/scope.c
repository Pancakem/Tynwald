#include "scope.h"
#include "symbol.h"
#include "hashtable.h"

/**
 * @brief: Initializes a new scope and returns a pointer to it.
 */
scope* init_scope(void) {
  scope* sc = malloc(sizeof(scope));
  
  sc->level = 0;
  sc->table = ht_new((eq_cb_t)strcmp, (hash_cb_t) symbol_hash);

  return sc;
}

/**
 * @brief: Creates a child to the current scope and returns a pointer to it.
 *
 * @param: parent_scope
 * @return: scope*
 */
scope* create_scope(scope* parent_scope) {
  scope* child = init_scope();
  child->parent_scope = malloc(sizeof (scope*));
  child->parent_scope = parent_scope;
  child->level = parent_scope->level + 1;
  
  return child;
}

/**
 * @brief adds a symbol to the current scope, returns comparison of the
 *        symbol and the result of inserting to the scope
 *         
 * @param scope
 * @param sym
 * @param int
 * @return int 
 */
int extend_scope(scope* scope, symbol sym) {
  char* key = sym.type == VAR_SYMBOL ? sym.val.var.name : sym.val.fun.name;

  if (ht_find(scope->table, &sym) == HT_NOTFOUND) {
    symbol* res = (symbol*) ht_insert(scope->table,
				      key,
				      (void*) &sym);
    return strcmp(key,
		  res->type == VAR_SYMBOL ? res->val.var.name : res->val.fun.name);
  }

  return ERR_SYMBOL_IN_SCOPE;
}

/**
 * @brief checks if a symbol is in scope if not found in the current
 *        scope, checks the parent scope. If there is no parent scope
 *        returns ERR_SYMBOL_NOT_FOUND
 *
 * @param scope*
 * @param symbol
 * @return int
 */
int in_scope(scope* scope, symbol sym) {
  symbol* res = ht_find(scope->table, &sym);

  if (res == HT_NOTFOUND) {
    if (scope->level == 0) {
      return ERR_SYMBOL_NOT_IN_SCOPE;
    }

    return in_scope(scope->parent_scope, sym);
  }

  return 1;  
}
