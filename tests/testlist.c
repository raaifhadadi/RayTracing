#include "../include/list.h"

#include <assert.h>

void testNew(void) {
  list_t *l = list_new();
  assert(l != NULL);
  assert(list_head(l) == NULL);
  assert(list_length(l) == 0);
  free(l);
}

void testDestroy(void) {
  list_t *l = list_new();
  list_destroy(l, NULL);
}

void testAdd(void) {
  int x, y, z;
  x = 1;
  y = 2;
  z = 3;

  list_t *l = list_new();

  list_add(l, &x);
  assert(list_length(l) == 1);
  list_add(l, &y);
  assert(list_length(l) == 2);
  list_add(l, &z);
  assert(list_length(l) == 3);

  list_destroy(l, NULL);
}

void testIterate(void) {
  int x, y, z;
  x = 1;
  y = 2;
  z = 3;

  list_t *l = list_new();
  list_add(l, &x);
  list_add(l, &y);
  list_add(l, &z);

  int sum = 0;
  for (void *v = list_iterate(l); v != NULL; v = list_iterate(NULL)) {
    sum += *(int *)v;
  }

  assert(sum == x + y + z);

  list_destroy(l, NULL);
}

void testIterateReentrant(void) {
  int x, y, z;
  x = 1;
  y = 2;
  z = 3;

  list_t *l = list_new();
  list_add(l, &x);
  list_add(l, &y);
  list_add(l, &z);

  int sum = 0;
  list_elem_t *le_ptr;
  for (void *v = list_iterate_r(l, &le_ptr); v != NULL;
       v = list_iterate_r(NULL, &le_ptr)) {
    sum += *(int *)v;
  }

  assert(sum == x + y + z);

  list_destroy(l, NULL);
}

int main(void) {
  testNew();
  testDestroy();
  testAdd();
  testIterate();
  testIterateReentrant();
  return EXIT_SUCCESS;
}
