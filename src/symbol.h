#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum {
  VOID,
  INT,
  CHAR,
  FLOAT,
  POINTER
} base_type;

typedef struct {
  base_type type;
  struct pointer_type* ptr;    /* allow multiple levels */
} pointer_type;

typedef union {
  base_type b_type;
  pointer_type p_type;
} type;

typedef enum {
  VAR_SYMBOL,
  FUN_SYMBOL,
} symbol_type;

typedef struct {
  symbol_type symbol_type;
  type type;
  char* name;
} var_symbol;

typedef struct {
  symbol_type symbol_type;
  type type;
  char* name;
  var_symbol* args;
} fun_symbol;

typedef union {
  fun_symbol fun;
  var_symbol var;
} symbol;
#endif
