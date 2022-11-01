#ifndef __VEC3_H_
#define __VEC3_H_

#include <stdbool.h>

/**
 * A vector in 3-dimensional Eucledian space.
 */
typedef struct Vec3 {
  double x;
  double y;
  double z;
} vec3_t;

/**
 * Returns a new vector struct whose x, y, z components are equale to a, b, c
 * respectively.
 */
vec3_t vec3_new(const double a, const double b, const double c);

/**
 * Basic vector algebra functions.
 */
vec3_t vec3_add(const vec3_t a, const vec3_t b);
vec3_t vec3_sub(const vec3_t a, const vec3_t b);
vec3_t vec3_mul(const vec3_t a, const double t);
vec3_t vec3_div(const vec3_t a, const double t);
double vec3_dot(const vec3_t a, const vec3_t b);
vec3_t vec3_cross(const vec3_t a, const vec3_t b);
double vec3_length(const vec3_t a);
double vec3_length_squared(const vec3_t a);
vec3_t vec3_reverse(const vec3_t v);
vec3_t vec3_normalize(const vec3_t a);

/**
 * Returns a vector whose components each take a random value between 0 and 1.
 */
vec3_t vec3_rand();

/**
 * Returns a vector whose components each take a random value between min and
 * max.
 */
vec3_t vec3_randRange(double min, double max);

/**
 * Returns a random vector in the unit sphere, i.e. a vector whose rotation
 * along the three directional axes is random and whose length is less than 1.
 */
vec3_t vec3_randInSphere();

/**
 * Returns a random vector ON the unit sphere, i.e. a vector whose rotation
 * along the three directional axes is random and whose length is equal to 1.
 */
vec3_t vec3_randUnit();

/**
 * Returns a random vector in the unit circle across the XY plane.
 * That is, the vector has a random rotation, a random length less than 1 and a
 * z component equal to 0.
 */
vec3_t vec3_randInDisc();

/**
 * Applies the given function to all three components of the vector, and updates
 * each component to each respective function result.
 */
vec3_t vec3_apply_func(const vec3_t a, double (*func)(double));

/**
 * Returns true if the given vector has length close to 0.
 */
bool vec3_close_to_zero(const vec3_t v);

/**
 * Reflects a given vector across a given normal.
 */
vec3_t vec3_reflect(const vec3_t v, const vec3_t n);

/**
 * Refracts a given vector across a given normal and eta ratio.
 */
vec3_t vec3_refract(const vec3_t v, const vec3_t n, double etaRatio);

/**
 * Compares two vectors pointwise, with accuracy DBL_EPSILON
 */
bool vec3_equals(const vec3_t v1, const vec3_t v2);

#endif  // __VEC3_H_
