#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stdlib.h>

typedef void *vec_t;

#define vec_erase(vec, pos, len)                                               \
  (_vec_erase((vec_t *)vec, sizeof(*vec), pos, len))
#define vec_remove(vec, pos) (_vec_remove((vec_t *)vec, sizeof(*vec), pos))

#define vec_copy(vec) (_vec_copy((vec_t *)vec, sizeof(*vec)))

vec_t vec_new(void);
void vec_destroy(vec_t vec);
vec_t _vec_add(vec_t *vec_addr, unsigned char type_size);
vec_t _vec_insert(vec_t *vec_addr, unsigned char type_size, size_t pos);
void _vec_erase(vec_t *vec_addr, unsigned char type_size, size_t pos,
                size_t len);
void _vec_remove(vec_t *vec_addr, unsigned char type_size, size_t pos);
void vec_pop(vec_t vec);
vec_t _vec_copy(vec_t vec, unsigned char type_size);
size_t vec_size(vec_t vec);
size_t vec_get_alloc(vec_t vec);

#endif
