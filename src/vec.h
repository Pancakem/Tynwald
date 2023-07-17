#ifndef VEC_H
#define VEC_H

#define VECTOR_INIT_SIZE 32

typedef struct {
  int size, capacity;
  void **data;
} vector_t;

void vector_new(vector_t *vector);
void vector_destroy(vector_t *vector);
int vector_size(vector_t *vector);
void vector_extend(vector_t *vector, int size);
void vector_append(vector_t *vector, void *value);
void *vector_at(vector_t *vector, int index);
void **vector_addrat(vector_t *vector, int index);

#endif
