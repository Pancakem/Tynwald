#ifndef SCOPE_H
#define SCOPE_H

#include "hashtable.h"
#include "symbol.h"

typedef struct {
  int level;
  struct scope* parent_scope;
  hashtable_t* table;
} scope;

scope* init_scope(void);
int extend_scope(scope* scope, symbol sym);
int remove_symbol(scope* scope, symbol sym);
int in_scope(scope* scope, symbol sym);
scope* create_scope(scope* parent_scope);
scope* destroy_scope(scope* scope);


#endif

