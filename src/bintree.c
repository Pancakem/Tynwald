#include "bintree.h"

btnode_t *btnode_alloc(void *key, void *value)
{
  btnode_t *node = (btnode_t *)malloc(sizeof(btnode_t));
  SYSEXPECT(node != NULL);
  node->key = key, node->value = value;
  node->left = node->right = NULL;
  return node;
}
void btnode_destroy(btnode_t *node) { free(node); }

void bt_new(bintree_t *bt, cmp_cb_t cmp)
{
  bt = (bintree_t *)malloc(sizeof(bintree_t));
  SYSEXPECT(bt != NULL);
  bt->cmp = cmp;
  bt->root = NULL;
  bt->size = 0;
}
void bt_destroy(bintree_t *bt)
{
  _bt_destroy(bt->root);
  free(bt);
}
void _bt_destroy(btnode_t *node)
{
  if (node == NULL)
    return;
  _bt_destroy(node->left);
  _bt_destroy(node->right);
  btnode_destroy(node);
}
void bt_str_new(bintree_t *bt) {
  bt_new(bt, strcmp_cb);
}

int bt_size(bintree_t *bt) { return bt->size; }

/**
 * @brief Insert the key, or return an existing key
 * 
 * @param bt 
 * @param key 
 * @param value 
 * @return void* 
 */
void *bt_insert(bintree_t *bt, void *key, void *value)
{
  btnode_t *found = NULL; // Set to new node if inserted, otherwise set to
  bt->root = _bt_insert(bt, bt->root, key, value, &found);
  return found->value;
}

btnode_t *_bt_insert(bintree_t *bt, btnode_t *node, void *key, void *value, btnode_t **found)
{
  if (node == NULL)
  {
    bt->size++;
    *found = btnode_alloc(key, value);
    return *found;
  } // Creates a new node
  int cmp = bt->cmp(key, node->key);
  if (cmp == 0)
    *found = node;
  else if (cmp < 0)
    node->left = _bt_insert(bt, node->left, key, value, found);
  else
    node->right = _bt_insert(bt, node->right, key, value, found);
  return node;
}

/**
 * @brief Return BT_NOTFOUND if not found, otherwise return the value
 * 
 * @param bt 
 * @param key 
 * @return void* 
 */
void *bt_find(bintree_t *bt, void *key) { return _bt_find(bt, bt->root, key); }
void *_bt_find(bintree_t *bt, btnode_t *node, void *key)
{
  if (node == NULL)
    return BT_NOTFOUND;
  int cmp = bt->cmp(key, node->key);
  if (cmp == 0)
    return node->value;
  else if (cmp < 0)
    return _bt_find(bt, node->left, key);
  else
    return _bt_find(bt, node->right, key);
}

/**
 * @brief Removes the given key, and returns the value if the key exists; otherwise return BT_NOTFOUND
 * 
 * @param bt 
 * @param key 
 * @return void* 
 */
void *bt_remove(bintree_t *bt, void *key)
{
  void *found = BT_NOTFOUND;
  bt->root = _bt_remove(bt, bt->root, key, &found);
  return found;
}

/**
 * @brief Returns the child after performing remove
 * 
 * @param bt 
 * @param node 
 * @param key 
 * @param found 
 * @return void* 
 */
void *_bt_remove(bintree_t *bt, btnode_t *node, void *key, void **found)
{
  if (node == NULL)
  {
    *found = BT_NOTFOUND;
    return NULL;
  }
  int cmp = bt->cmp(key, node->key);
  if (cmp == 0)
  {
    *found = node->value;
    bt->size--;
    return _bt_remove_node(bt, node);
  }
  else if (cmp < 0)
    node->left = _bt_remove(bt, node->left, key, found);
  else
    node->right = _bt_remove(bt, node->right, key, found);
  return node;
}

/**
 * @brief Internal function only called by bt_remove()
 * 
 * @param bt 
 * @param node 
 * @return void* 
 */
void *_bt_remove_node(bintree_t *bt, btnode_t *node)
{
  btnode_t *left = node->left, *right = node->right;
  if (left == NULL)
  {
    btnode_destroy(node);
    return right;
  } // This also covers the leaf node case
  else if (right == NULL)
  {
    btnode_destroy(node);
    return left;
  }
  if (right->left == NULL)
  {
    btnode_destroy(node);
    right->left = left;
    return right;
  }
  do
  {
    left = right;
    right = right->left;
  } while (right->left);
  node->key = right->key;
  node->value = right->value;
  left->left = right->right;
  btnode_destroy(right);
  return node;
}