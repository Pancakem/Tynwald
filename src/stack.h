
#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define STACK_INIT_CAPACITY 128

// Implements a general stack which is used in the shift-reduce parsing algo.
typedef struct {
  size_t size;
  size_t capacity;
  void **data;
} tynwald_stack_t;

tynwald_stack_t *stack_new(void);
void stack_destroy(tynwald_stack_t *stack);
void stack_push(tynwald_stack_t *stack, void *p);
void *stack_pop(tynwald_stack_t *stack);
void *stack_peek(tynwald_stack_t *stack);
void *stack_peek_at(tynwald_stack_t *stack, size_t offset);
void *stack_at(tynwald_stack_t *stack, size_t index);
int stack_empty(tynwald_stack_t *stack);
int stack_size(tynwald_stack_t *stack);
void **stacktopaddr(tynwald_stack_t *stack);

#endif
