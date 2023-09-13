#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "symbol.h"

typedef struct {
  int level;
  
  struct scope* parentScope;
} scope;

scope* init_scope();
int add_symbol_to_scope(scope *scope, symbol sym);
int remove_from_scope(scope *scope, char *identifier);
int in_scope(scope *scope, char *identifier);
scope* create_child_scope(scope *parentScope);
scope* delete_scope(scope *scope);

#endif
