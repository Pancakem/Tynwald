
#ifndef BIN_TREE_H
#define BIN_TREE_H

#include "hashtable.h" // Need its def for call back functions

#define BT_NOTFOUND ((void *)-1)

// Binary tree node type
typedef struct btnode {
  void *key, *value;
  struct btnode *left, *right;
} btnode_t;

// The good thing about a binary tree search structure is that the physical size
// grows proportionally with the logical size, which is desirable for structures
// that are usually small, but sometimes huge
typedef struct {
  int size;
  cmp_cb_t cmp;
  btnode_t *root;
} bintree_t;

btnode_t *btnode_alloc(void *key, void *value);
void btnode_destroy(btnode_t *node);
void bt_new(bintree_t *bt, cmp_cb_t cmp);
void bt_destroy(bintree_t *bt);
void _bt_destroy(btnode_t *node);
void bt_str_new(bintree_t *bt);
int bt_size(bintree_t *bt);
void *bt_insert(bintree_t *bt, void *key, void *value);
btnode_t *_bt_insert(bintree_t *bt, btnode_t *node, void *key, void *value, btnode_t **found);
void *bt_find(bintree_t *bt, void *key);
void *_bt_find(bintree_t *bt, btnode_t *node, void *key);
void *bt_remove(bintree_t *bt, void *key);
void *_bt_remove(bintree_t *bt, btnode_t *node, void *key, void **found);
void *_bt_remove_node(bintree_t *bt, btnode_t *node);

#endif