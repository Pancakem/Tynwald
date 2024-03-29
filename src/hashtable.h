
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

// Must be a power of two
#define HT_INIT_CAPACITY 128
#define HT_INIT_MASK 0x7F
#define HT_RESIZE_THRESHOLD(capacity) (capacity / 8 * 7)
#define HT_NOTFOUND ((void *)-1)
#define HT_REMOVED  ((void *)-2)

#define HT_OP_INSERT 0
#define HT_OP_FIND   1

typedef unsigned long hashval_t;
typedef int (*eq_cb_t)(void *, void *);    // Equality comparison function
typedef int (*cmp_cb_t)(void *, void *);   // Comparison comparison function
typedef hashval_t (*hash_cb_t)(void *);    // Hash value function

typedef struct {
  eq_cb_t eq;
  hash_cb_t hash;
  hashval_t mask;
  size_t size;
  size_t capacity;
  void **keys;
  void **values;
} hashtable_t;

int streq_cb(void *a, void *b);
int strcmp_cb(void *a, void *b);
hashval_t strhash_cb(void *a);
hashtable_t *ht_new(eq_cb_t eq, hash_cb_t hash);
hashtable_t *ht_str_new(void);
void ht_destroy(hashtable_t *ht);
int ht_size(hashtable_t *ht);
int ht_find_slot(hashtable_t *ht, void **keys, void *key, int op);
void ht_resize(hashtable_t *ht);
void *ht_find(hashtable_t *ht, void *key);
void *ht_insert(hashtable_t *ht, void *key, void *value);
void *ht_remove(hashtable_t *ht, void *key);

typedef hashtable_t set_t; // Set is just a hash table (we waste some space)
#define SET_FAIL     0
#define SET_SUCCESS  1
#define set_new(a, b) ht_new(a, b)
#define set_str_new() ht_str_new()
#define set_destroy(a) ht_destroy(a)
#define set_size(a) ht_size(a)
int set_find(set_t *set, void *key);
int set_insert(set_t *set, void *key);
int set_remove(set_t *set, void *key);

#endif
