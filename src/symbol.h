#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum {
  VOID,
  INT,
  CHAR,
  FLOAT,
  POINTER
} base_type;

typedef union {
  base_type b_type;
  /* TODO: figure out how to represent the pointer type */
} type;

typedef enum {
  VAR_SYMBOL,
  FUN_SYMBOL,
} symbol_type;

typedef struct {
  type type;
  char* name;
} var_symbol;

typedef struct {
  type type;
  char* name;
  var_symbol* args;
} fun_symbol;

typedef struct {
  symbol_type symbol_type;
  union {
    var_symbol v_sym;
    fun_symbol f_sym;
  } symbol_val;
} symbol;

#endif
