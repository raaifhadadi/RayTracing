#ifndef __RAY_H_
#define __RAY_H_

#include "vec3.h"

/**
 * A ray cast into a scene from a camera.
 */
typedef struct Ray {
  vec3_t origin;
  vec3_t direction;
} ray_t;

/**
 * Returns the position of a ray after it has travelled for t units in 3D space.
 */
vec3_t ray_pos(const ray_t *r, double t);

#endif  // __RAY_H_
