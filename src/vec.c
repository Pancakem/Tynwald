#include "vec.h"
#include <string.h>

#define GROWTH_FACTOR 2

typedef struct vec_data vec_data;

struct vec_data {
  unsigned char alloc;
  unsigned char length;
  char data[]; // bytes of stored info
};

vec_data *vec_allocate(unsigned char alloc, unsigned char type_size) {
  vec_data *vd = (vec_data *)malloc(sizeof(vec_data) + alloc * type_size);
  vd->alloc = alloc;
  return vd;
}

vec_data *vec_realloc(vec_data *vd, unsigned char type_size) {
  unsigned char new_alloc = (vd->alloc == 0) ? 1 : vd->alloc * GROWTH_FACTOR;
  vec_data *new_vd =
      (vec_data *)realloc(vd, sizeof(vec_data) + new_alloc * type_size);

  new_vd->alloc = new_alloc;
  return new_vd;
}

vec_data *vec_get_data(vec_t vec) { return &((vec_data *)vec)[-1]; }

vec_t vec_new(void) {
  vec_data *vd = (vec_data *)malloc(sizeof(vec_data));
  vd->alloc = 0;
  vd->length = 0;

  return &vd->data;
}

void vec_destroy(vec_t vec) { free(vec_get_data(vec)); }

size_t vec_size(vec_t vec) { return vec_get_data(vec)->alloc; }

bool vec_has_slot(vec_data *vd) { return (vd->alloc - vd->length) > 0; }

void *_vec_add(vec_t *vec_addr, unsigned char type_size) {
  vec_data *vd = vec_get_data(*vec_addr);

  if (!vec_has_slot(vd)) {
    vd = vec_realloc(vd, type_size);
    *vec_addr = vd->data;
  }

  return (void *)&vd->data[type_size * vd->length++];
}

vec_t _vec_insert(vec_t *vec_addr, unsigned char type_size, size_t pos) {
  vec_data *vd = vec_get_data(*vec_addr);
  unsigned char new_length = vd->length++;

  if (!vec_has_slot(vd)) {
    vd = vec_realloc(vd, type_size);
    *vec_addr = vd->data;
  }

  memmove(&vd->data[(pos + 1) * type_size], &vd->data[pos * type_size],
          (vd->length - pos) * type_size);

  vd->length = new_length;
  return &vd->data[pos * type_size];
}

/**
 * @note The function uses memmove putting in a bad address is A BIG GUN
 */
void _vec_erase(vec_t *vec_addr, unsigned char type_size, size_t pos,
                size_t len) {
  vec_data *vd = vec_get_data(*vec_addr);
  memmove(&vd->data[pos * type_size], &vd->data[(pos + len) * type_size],
          (vd->length - pos - len) * type_size);
  vd->length -= len;
}

void _vec_remove(vec_t *vec_addr, unsigned char type_size, size_t pos) {
  _vec_erase(vec_addr, type_size, pos, 1);
}

void vec_pop(vec_t vec) { --vec_get_data(vec)->length; }

vec_t _vec_copy(vec_t vec, unsigned char type_size) {
  vec_data *vd = vec_get_data(vec);
  size_t alloc_size = sizeof(vec_data) + vd->length * type_size;
  vec_data *v = (vec_data *)malloc(alloc_size);
  memcpy(v, vd, alloc_size);
  return (void *)&v->data;
}
