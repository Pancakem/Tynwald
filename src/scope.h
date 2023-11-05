#ifndef SCOPE_H
#define SCOPE_H

#include "hashtable.h"
#include "symbol.h"

#define ERR_SYMBOL_IN_SCOPE -5
#define ERR_SYMBOL_NOT_IN_SCOPE 0

struct scope_ {
  int level;
  struct scope_* parent_scope;
  hashtable_t* table;
};

typedef struct scope_ scope;

scope* init_scope(void);
int extend_scope(scope* scope, symbol sym);
int remove_symbol(scope* scope, symbol sym);
int in_scope(scope* scope, symbol sym);
scope* create_scope(scope* parent_scope);
scope* destroy_scope(scope* scope);


#endif

