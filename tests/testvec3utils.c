#include "../include/vec3.h"
#include "../include/utils.h"
#include <assert.h>
#include <math.h>

void test_add() {
  vec3_t v1 = (vec3_t){1, 2, 3};
  vec3_t v2 = (vec3_t){0, 0, 0};

  vec3_t vRes = vec3_add(v1, v2);
  assert(vec3_equals(v1, vRes));

  v2 = (vec3_t){-3, -2, -1};
  vRes = vec3_add(v1, v2);
  vec3_t vsol = (vec3_t){-2, 0, 2};
  assert(vec3_equals(vRes, vsol));
}

void test_sub() {
  vec3_t v1 = (vec3_t){1, 2, 3};
  vec3_t v2 = (vec3_t){0, 0, 0};

  vec3_t vRes = vec3_sub(v1, v2);
  assert(vec3_equals(v1, vRes));

  v2 = (vec3_t){-3, -2, -1};
  vRes = vec3_sub(v1, v2);
  vec3_t vsol = (vec3_t){4, 4, 4};
  assert(vec3_equals(vRes, vsol));
}

void test_mul() {
  vec3_t v1 = (vec3_t){1, 2, 3};
  vec3_t vRes = vec3_mul(v1, 0);
  vec3_t vSol = (vec3_t){0, 0, 0};
  assert(vec3_equals(vRes, vSol));

  vRes = vec3_mul(v1, 2);
  vSol = (vec3_t){2, 4, 6};
  assert(vec3_equals(vRes, vSol));

  vRes = vec3_mul(v1, -3);
  vSol = (vec3_t){-3, -6, -9};
  assert(vec3_equals(vRes, vSol));
}

void test_div() {
  vec3_t v1 = (vec3_t){6, 12, 18};
  vec3_t vRes = vec3_div(v1, 2);
  vec3_t vSol = (vec3_t){3, 6, 9};
  assert(vec3_equals(vRes, vSol));

  vRes = vec3_div(v1, 6);
  vSol = (vec3_t){1, 2, 3};
  assert(vec3_equals(vRes, vSol));

  vRes = vec3_div(v1, 10);
  vSol = (vec3_t){0.6, 1.2, 1.8};
  assert(vec3_equals(vRes, vSol));

  vRes = vec3_div(v1, 11);
  vSol = (vec3_t){6.0 / 11.0, 12.0 / 11.0, 18.0 / 11.0};
  assert(vec3_equals(vRes, vSol));
}

void test_dot() {
  vec3_t v1 = (vec3_t){1, 2, 3};
  vec3_t v2 = (vec3_t){4, 5, 6};

  assert(vec3_dot(v1, v2) == (1 * 4 + 2 * 5 + 3 * 6));

  v1.x = -3;
  v1.y = 0;
  v2.x = 1;
  v2.z = 0;
  assert(vec3_dot(v1, v2) == -3);
}

void test_cross() {
  vec3_t v1 = (vec3_t){1, 2, 3};
  vec3_t v2 = (vec3_t){4, 5, 6};
  vec3_t vRes = vec3_cross(v1, v2);
  vec3_t vSol = (vec3_t){-3, 6, -3};
  assert(vec3_equals(vRes, vSol));

  v2 = (vec3_t){23, 12, 17};
  vRes = vec3_cross(v1, v2);
  vSol = (vec3_t){-2, 52, -34};
  assert(vec3_equals(vRes, vSol));
}

void test_length() {
  vec3_t v1 = (vec3_t){23, 12, 17};
  assert(double_equals(vec3_length(v1), sqrt(23 * 23 + 12 * 12 + 17 * 17)));

  v1 = (vec3_t){-20, 19, -13};
  assert(double_equals(vec3_length(v1), sqrt(20 * 20 + 19 * 19 + 13 * 13)));
}

void test_normalize() {
  vec3_t v1 = (vec3_t){23, 12, 17};
  vec3_t vRes = vec3_normalize(v1);
  vec3_t vSol = {23.0 / sqrt(962), 12.0 / sqrt(962), 17.0 / sqrt(962)};
  assert(vec3_equals(vRes, vSol));
}

int main(void) {
  test_add();
  test_sub();
  test_mul();
  test_div();
  test_dot();
  test_cross();
  test_length();
  test_normalize();
  return 0;
}
