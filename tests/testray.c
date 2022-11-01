#include "../include/ray.h"
#include <assert.h>

void testRay_pos(void) {
  ray_t r;
  r.origin = (vec3_t){0, 0, 0};
  r.direction = (vec3_t){1, 2, 3};
  vec3_t vRes = ray_pos(&r, 10.5);
  vec3_t vSol = (vec3_t){10.5, 21, 31.5};
  assert(vec3_equals(vRes, vSol));
}

int main(void) { return 0; }
