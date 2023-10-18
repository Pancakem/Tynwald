#include "stack.h"
#include "error.h"

tynwald_stack_t *stack_new(void) {
  tynwald_stack_t *stack = (tynwald_stack_t *)malloc(sizeof(tynwald_stack_t));
  SYSEXPECT(stack != NULL);
  stack->data = (void **)malloc(sizeof(void *) * STACK_INIT_CAPACITY);
  SYSEXPECT(stack->data != NULL);
  stack->size = 0;
  stack->capacity = STACK_INIT_CAPACITY;
  return stack;
}

void stack_destroy(tynwald_stack_t *stack) {
  free(stack->data);
  free(stack);
}

void stack_push(tynwald_stack_t *stack, void *p) {  
  if(stack->size == stack->capacity) {
    void **old = stack->data;
    stack->data = malloc(sizeof(void *) * stack->capacity * 2);
    SYSEXPECT(stack->data != NULL);
    memcpy(stack->data, old, sizeof(void *) * stack->capacity);
    stack->capacity *= 2;
    free(old);
  }
  assert(stack->size < stack->capacity);
  stack->data[stack->size++] = p;
}

void *stack_pop(tynwald_stack_t *stack) {
  assert(stack->size != 0);
  return stack->data[--stack->size];
}

void *stack_peek(tynwald_stack_t *stack) {
  assert(stack->size != 0);
  return stack->data[stack->size - 1];
}

// Offset is from the top of the stack towards the bottom
void *stack_peek_at(tynwald_stack_t *stack, size_t offset) {
  assert(offset < stack->size);
  return stack->data[stack->size - 1 - offset];
}

void *stack_at(tynwald_stack_t *stack, size_t index) {
  assert(index < stack->size);
  return stack->data[index];
}

void **stacktopaddr(tynwald_stack_t *stack) {
  return stack->data + stack->size;
}

int stack_empty(tynwald_stack_t *stack) { return stack->size == 0; }
int stack_size(tynwald_stack_t *stack) { return stack->size; }
