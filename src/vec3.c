#include "../include/vec3.h"

#include <math.h>

#include "../include/utils.h"

#define DELTA 0.0000001

vec3_t vec3_new(const double a, const double b, const double c) {
  return (vec3_t){a, b, c};
}

vec3_t vec3_add(const vec3_t a, const vec3_t b) {
  vec3_t r;
  r.x = a.x + b.x;
  r.y = a.y + b.y;
  r.z = a.z + b.z;
  return r;
}

vec3_t vec3_sub(const vec3_t a, const vec3_t b) {
  vec3_t r;
  r.x = a.x - b.x;
  r.y = a.y - b.y;
  r.z = a.z - b.z;
  return r;
}

vec3_t vec3_mul(const vec3_t a, const double t) {
  vec3_t r;
  r.x = a.x * t;
  r.y = a.y * t;
  r.z = a.z * t;
  return r;
}

vec3_t vec3_div(const vec3_t a, const double t) { return vec3_mul(a, 1.0 / t); }

double vec3_dot(const vec3_t a, const vec3_t b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3_cross(const vec3_t a, const vec3_t b) {
  vec3_t r;
  r.x = a.y * b.z - a.z * b.y;
  r.y = a.z * b.x - a.x * b.z;
  r.z = a.x * b.y - a.y * b.x;
  return r;
}

double vec3_length(const vec3_t a) { return sqrt(vec3_length_squared(a)); }

double vec3_length_squared(const vec3_t a) { return vec3_dot(a, a); }

vec3_t vec3_normalize(const vec3_t a) { return vec3_div(a, vec3_length(a)); }

vec3_t vec3_reverse(const vec3_t v) { return vec3_mul(v, -1); }

vec3_t vec3_rand() {
  vec3_t r;
  r.x = rand_double();
  r.y = rand_double();
  r.z = rand_double();
  return r;
}

vec3_t vec3_randRange(double min, double max) {
  vec3_t r;
  r.x = rand_double_range(min, max);
  r.y = rand_double_range(min, max);
  r.z = rand_double_range(min, max);
  return r;
}

vec3_t vec3_randInSphere() {
  return vec3_mul(vec3_normalize(vec3_randRange(-1000.0, 1000.0)),
                  rand_double());
}

vec3_t vec3_randUnit() { return vec3_normalize(vec3_randInSphere()); }

vec3_t vec3_randInDisc() {
  vec3_t r;
  double theta = rand_double_range(0.0, 2.0) * PI;
  r.x = cos(theta);
  r.y = sin(theta);
  r.z = 0.0;
  return vec3_mul(r, rand_double());
}

vec3_t vec3_apply_func(const vec3_t a, double (*func)(double)) {
  vec3_t r;
  r.x = func(a.x);
  r.y = func(a.y);
  r.z = func(a.z);
  return r;
}

bool vec3_close_to_zero(const vec3_t v) {
  double d = DELTA;
  return (v.x < d) && (v.y < d) && (v.z < d);
}

vec3_t vec3_reflect(const vec3_t v, const vec3_t n) {
  return vec3_sub(v, vec3_mul(n, 2 * vec3_dot(v, n)));
}

vec3_t vec3_refract(const vec3_t v, const vec3_t n, double etaRatio) {
  double cos_theta = fmin(vec3_dot(vec3_mul(v, -1), n), 1.0);
  vec3_t perp = vec3_mul(vec3_add(v, vec3_mul(n, cos_theta)), etaRatio);
  vec3_t parallel = vec3_mul(n, -sqrt(fabs(1.0 - vec3_length_squared(perp))));
  return vec3_add(perp, parallel);
}

bool vec3_equals(const vec3_t v1, const vec3_t v2) {
  return double_equals(v1.x, v2.x) && double_equals(v1.y, v2.y) &&
         double_equals(v1.z, v2.z);
}
