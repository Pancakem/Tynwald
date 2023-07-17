
#include "list.h"

void LIST_SIMPLE_FREE_CB(void *p) {
  free(p);
}

void list_new(list_t *list) {
  list = (list_t *)malloc(sizeof(list_t));
  SYSEXPECT(list != NULL);
  list->size = 0;
  list->head = list->tail = NULL;
}

void list_destroy(list_t *list) {
  assert(list->head || !list->tail);
  listnode_t *node = list->head;
  while(node != NULL) {
    listnode_t *next = node->next;
    if(list->key_destroy_cb != NULL) {
      list->key_destroy_cb(node->key);
    }
    if(list->value_destroy_cb != NULL) {
      list->value_destroy_cb(node->value);
    }
    listnode_destroy(node);
    node = next;
  }
  free(list);
}

void list_set_destroy_cb(list_t *list, void (*key_destroy_cb)(void *), void (*value_destroy_cb)(void *)) {
  list->key_destroy_cb = key_destroy_cb;
  list->value_destroy_cb = value_destroy_cb;
}

int list_size(list_t *list) { 
  return list->size; 
}

/**
 * @brief Allocate a node. All fields are uninitialized
 * 
 * @return listnode_t* 
 */
listnode_t *listnode_alloc(void) {
  listnode_t *node = (listnode_t *)malloc(sizeof(listnode_t));
  SYSEXPECT(node != NULL);
  return node;
}
void listnode_destroy(listnode_t *node) { 
  free(node);
}

/**
 * @brief Always insert to the end of the list; do not check for duplicate; Always return the inserted value
 * 
 * @param list 
 * @param key 
 * @param value 
 * @return void* 
 */
void *list_insert(list_t *list, void *key, void *value) {
  listnode_t *node = listnode_alloc();
  node->key = key;
  node->value = value;
  node->next = NULL;
  assert(list->head || !list->tail);  // If head is NULL then tail must also be NULL
  if(list->head == NULL) {
    list->head = list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }
  list->size++;
  return value;
}

/**
 * @brief Inserts before the node specified by index; if index == list size then insert at the end
 * 
 * @param list 
 * @param key 
 * @param value 
 * @param index 
 * @return listnode_t* 
 */
listnode_t *list_insertat(list_t *list, void *key, void *value, int index) {
  assert(index <= list->size && index >= 0);
  if(index == list->size) {
    return list_insert(list, key, value); // Empty insert will be caught here
  }
  assert(list->size > 0);
  list->size++;
  listnode_t *node = listnode_alloc();
  node->key = key;
  node->value = value;
  if(index == 0) {
    node->next = list->head;
    list->head = node;
    assert(list->tail);
  } else {
    listnode_t *curr = list->head;
    while(--index != 0) curr = curr->next;
    node->next = curr->next;
    curr->next = node;
    assert(curr->next);
  }
  return value;
}

void *list_insert_nodup(list_t *list, void *key, void *value, eq_cb_t eq) {
  void *ret = list_find(list, key, eq);
  if(ret == LIST_NOTFOUND) {
    value = list_insert(list, key, value);
  }
  return ret;
}

/**
 * @brief Search for the given key, and return value; Return LIST_NOTFOUND if not found
 * 
 * @param list 
 * @param key 
 * @param eq 
 * @return void* 
 */
void *list_find(list_t *list, void *key, eq_cb_t eq) {
  listnode_t *curr = list->head;
  while(curr != NULL) {
    if(eq(key, curr->key)) {
      return curr->value;
    } else { 
      curr = curr->next;
    }
  }
  return LIST_NOTFOUND;
}

/**
 * @brief Returns the node specified by the index; If index is too large then return LIST_NOTFOUND. 
          Index must be positive
 * 
 * @param list 
 * @param index 
 * @return const listnode_t* 
 */
const listnode_t *list_findat(list_t *list, int index) {
  assert(index >= 0);
  if(index >= list->size) {
    return LIST_NOTFOUND;
  }
  listnode_t *curr = list->head;
  while(index-- != 0) {
    curr = curr->next;
  }
  return curr;
}

/**
 * @brief Removes the key from the list. Return value if key exists; LIST_NOTFOUND otherwise
 * 
 * @param list 
 * @param key 
 * @param eq 
 * @return void* 
 */
void *list_remove(list_t *list, void *key, eq_cb_t eq) {
  listnode_t *curr = list->head;
  listnode_t *prev = curr;
  if(curr == NULL) {
    return LIST_NOTFOUND;
  }
  void *ret = NULL;
  if(eq(curr->key, key)) {
    list->head = curr->next;  // Could be NULL
    ret = curr->value;
    listnode_destroy(curr);
    list->size--;
    if(curr == list->tail) {
      list->tail = NULL;
    }
    return ret;
  }
  do {
    curr = curr->next;
    if(curr != NULL && eq(curr->key, key)) {
      prev->next = curr->next;
      ret = curr->value;
      listnode_destroy(curr);
      list->size--;
      if(curr == list->tail) {
        list->tail = prev; // If deleting the last element then adjust tail
      }
      return ret;
    }
    prev = curr;
  } while(curr);
  return LIST_NOTFOUND;
}

/**
 * @brief Value is returned, and the second argument holds the key
 * 
 * @param list 
 * @param index 
 * @param key 
 * @return void* 
 */
void *list_removeat(list_t *list, int index, void **key) {
  assert(index >= 0);
  if(index >= list->size) return LIST_NOTFOUND;
  list->size--;
  listnode_t *curr = list->head, *prev = NULL;
  void *ret = NULL;
  if(index == 0) { 
    list->head = curr->next; 
  } else {
    while(index--) {
      prev = curr;
      curr = curr->next;
    }
    prev->next = curr->next;
  }
  ret = curr->value;
  *key = curr->key;
  listnode_destroy(curr);
  if(curr == list->tail) {
    list->tail = prev;
  }
  return ret;
}