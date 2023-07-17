#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "util.h"
#include "error.h"

/**
 * @brief Read a file into a buffer
 * @note It is upon the user to free the buffer
 * @param file_path
 * @return char*
 */
char *read_file(const char *file_path)
{
    FILE *fle = fopen(file_path, "r");
    if (!fle)
        return NULL;

    fseek(fle, 0, SEEK_END);
    long size = ftell(fle);
    fseek(fle, 0, SEEK_SET);
    char *buffer = (char *)malloc(sizeof(char) * size + 1);
    fread(buffer, 1, size, fle);
    buffer[size] = '\0';
    fclose(fle);
    return buffer;
}

/**
 * @brief String factory function
 * 
 * @param str
 * @return void 
 */
void str_new(str_t *str) {
  str = (str_t *)malloc(sizeof(str_t));
  SYSEXPECT(str != NULL);
  str->s = (char *)malloc(STR_INIT_SIZE + 1);
  SYSEXPECT(str->s != NULL);
  str->capacity = STR_INIT_SIZE;
  str->size = 0;
  str->s[0] = '\0';
}

/**
 * @brief Free string
 * 
 * @param str 
 */
void str_destroy(str_t *str) { free(str->s); free(str); }

/**
 * @brief Clear a string without freeing it
 * 
 * @param str 
 */
void str_clear(str_t *str) { str->s[0] = '\0'; str->size = 0; }

int str_size(str_t *str) { return str->size; }

/**
 * @brief Realloc the buffer to hold at least size + 1 bytes
 * 
 * @param str 
 * @param size 
 */
void str_extend(str_t *str, int size) {
  if(size > str->capacity) {
    str->s = realloc(str->s, size + 1);
    SYSEXPECT(str->s != NULL);
    str->capacity = size;
  }
}

/**
 * @brief Add character to end of string
 *  @note Increases size if full
 * @param str 
 * @param ch 
 */
void str_append(str_t *str, char ch) {
  if(str->size == str->capacity) str_extend(str, str->capacity * 2);
  assert(str->size < str->capacity);
  str->s[str->size++] = ch;
  str->s[str->size] = '\0';
}


/**
 * @brief Add character to the start of the string
 * @note Increases size if full
 * @param str 
 * @param ch 
 */
void str_prepend(str_t *str, char ch) {
  if(str->size == str->capacity) str_extend(str, str->capacity * 2);
  assert(str->size < str->capacity);
  memmove(str->s + 1, str->s, str->size + 1); // Including the trailing zero
  str->s[0] = ch;
  str->size++;
}

/**
 * @brief Add string to the start of the string
 * @note Increases size if full
 * @param str 
 * @param s 
 */
void str_prepend_str(str_t *str, const char *s) {
  int copylen = strlen(s);
  if(str->size + copylen >= str->capacity) str_extend(str, str->size + copylen);
  assert(str->size + copylen <= str->capacity);
  memmove(str->s + copylen, str->s, str->size + 1); // Including the trailing zero
  memcpy(str->s, s, copylen); // Do not include the trailing zero
  str->size += copylen;
}

/**
 * @brief Concatenate string
 * @note Increase size if full
 * @param str 
 * @param s 
 */
void str_concat(str_t *str, const char *s) {
  int copylen = strlen(s);
  if(str->size + copylen >= str->capacity) str_extend(str, str->size + copylen);
  assert(str->size + copylen <= str->capacity);
  memmove(str->s + str->size, s, copylen + 1); // Includes the '\0'
  str->size += copylen;
}

/**
 * @brief Append in to string
 * 
 * @param str 
 * @param d 
 */
void str_print_int(str_t *str, int d) {
  char temp[MAX_INT_DIGITS];
  sprintf(temp, "%d", d);
  str_concat(str, temp);
}

/**
 * @brief Returns a string allocated from heap. The str is not changed
 * @note Allocates dest on the heap
 * @param str 
 * @return char* 
 */
void str_copy(char *dest, const str_t *src) {
  dest = (char *)malloc(src->size + 1);
  SYSEXPECT(dest != NULL);
  memcpy(dest, src->s, src->size + 1);
}