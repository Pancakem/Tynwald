#ifndef VEC_H
#define VEC_H

#include <stddef.h>

#define VECTOR_INIT_SIZE 32

typedef struct {
  size_t size, capacity;
  void **data;
} vector_t;

vector_t * vector_new(void);
void vector_destroy(vector_t *vector);
int vector_size(vector_t *vector);
void vector_extend(vector_t *vector, size_t size);
void vector_append(vector_t *vector, void *value);
void *vector_at(vector_t *vector, size_t index);
void **vector_addrat(vector_t *vector, size_t index);

#endif
