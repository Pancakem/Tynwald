#include "symbol.h"
#include "hashtable.h"

hashval_t symbol_hash(symbol* sym) {
  if (sym->type == VAR_SYMBOL)
    return strhash_cb(sym->val.var.name);

  var_symbol* arg_ptr = sym->val.fun.args;
  char* str_to_hash = "";
  while (arg_ptr) {   // assuming not a zero arg function
    str_to_hash = strncat(str_to_hash, arg_ptr->name,
			  strlen(arg_ptr->name));
    arg_ptr++;
  }

  strncat(str_to_hash, sym->val.fun.name, strlen(sym->val.fun.name));

  return strhash_cb(str_to_hash);
}
