#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

#define STR_INIT_SIZE 32     // Excluding the terminating 0
#define MAX_INT_DIGITS 64    // Can't be that long...

typedef struct {
  int size;
  int capacity;          // Both excluding the terminating 0
  char *s;
} str_t;

void str_new(str_t *str);
void str_destroy(str_t *str);
void str_clear(str_t *str);
int str_size(str_t *str);
void str_extend(str_t *str, int size);
void str_append(str_t *str, char ch);
void str_prepend(str_t *str, char ch);
void str_concat(str_t *str, const char *s);
void str_prepend_str(str_t *str, const char *s);
void str_print_int(str_t *str, int d);
void str_copy(char *dest, const str_t *src);
static inline char *str_cstr(str_t *s) { return s->s; }

char *read_file(const char *file_path);


#endif