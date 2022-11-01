#include "../include/list.h"

list_t *list_new() {
  list_t *l = malloc(sizeof(list_t));
  if (l == NULL) return NULL;
  l->head = NULL;
  l->curr = NULL;
  l->length = 0;
  return l;
}

void list_destroy(list_t *l, val_free_func val_free) {
  list_elem_t *curr;
  list_elem_t *next = l->head;
  while (next != NULL) {
    curr = next;
    next = curr->next;
    if (val_free != NULL) {
      val_free(curr->value);
    }
    free(curr);
  }
  free(l);
}

void list_add(list_t *l, void *val) {
  list_elem_t *new_node = malloc(sizeof(list_elem_t));
  new_node->value = val;
  new_node->next = l->head;
  l->head = new_node;
  ++l->length;
}

void *list_head(list_t *l) {
  if (l->head) return l->head->value;
  return NULL;
}

int list_length(list_t *l) { return l->length; }

void *list_iterate_r(list_t *l, list_elem_t **le_ptr) {
  if (l) *le_ptr = l->head;
  void *ret = NULL;
  if (*le_ptr) {
    ret = (*le_ptr)->value;
    *le_ptr = (*le_ptr)->next;
  }
  return ret;
}

void *list_iterate(list_t *l) {
  static list_elem_t *le = NULL;
  if (l) le = l->head;
  void *ret = NULL;
  if (le) {
    ret = le->value;
    le = le->next;
  }
  return ret;
}
