#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "vec.h"
#include "error.h"

void vector_new(vector_t *vector) {
  vector = (vector_t *)malloc(sizeof(vector_t));
  SYSEXPECT(vector != NULL);
  vector->data = (void **)malloc(VECTOR_INIT_SIZE * sizeof(void *));
  SYSEXPECT(vector->data != NULL);
  vector->size = 0;
  vector->capacity = VECTOR_INIT_SIZE;
}
void vector_destroy(vector_t *vector) { free(vector->data); free(vector); }
int vector_size(vector_t *vector) { return vector->size; }

void vector_extend(vector_t *vector, int size) {
  if(size > vector->capacity) {
    vector->capacity = size;
    vector->data = realloc(vector->data, size * sizeof(void *));
    SYSEXPECT(vector->data != NULL);
  }
}

void vector_append(vector_t *vector, void *value) {
  if(vector->size == vector->capacity) vector_extend(vector, vector->size * 2);
  assert(vector->size < vector->capacity);
  vector->data[vector->size++] = value;
}

void *vector_at(vector_t *vector, int index) {
  assert(index < vector->size && index >= 0);
  return vector->data[index];
}

void **vector_addrat(vector_t *vector, int index) {
  assert(index < vector->capacity && index >= 0);  // Since we only take address, use capacity here
  return vector->data + index;
}