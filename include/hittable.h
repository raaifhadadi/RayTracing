#ifndef __HIT_RECORD__
#define __HIT_RECORD__
struct HitRecord;
typedef struct HitRecord hit_record_t;
struct Hittable;
typedef struct Hittable hittable_t;
typedef struct Hittable hittable_list_t;
#endif  // __HIT_RECORD__

#ifndef __HITTABLE_H__
#define __HITTABLE_H__

#include "vec3.h"
#include "ray.h"
#include "list.h"
#include "material.h"

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

/**
 * Data about the collision between a ray and a hittable object in the world.
 *
 * point - the precise point of the collision
 * mat - the material of the hit object
 * normal - the normal of the surface which was hit by the ray
 * t - the length that the colliding ray has travelled to reach this object.
 * front_face - true if the ray hit the outside of the object, i.e. the surface
 *              normal makes an acute angle with the ray. False otherwise.
 */
struct HitRecord {
  vec3_t point;
  material_t *mat;
  vec3_t normal;
  double t;
  bool front_face;
};

/**
 * An object which can be hit by a ray.
 *
 * mat - the material of the object. Determines the object's appearance in the
 *       final render. Cannot be NULL.
 * hit_func - pointer to the function which determines the behaviour of a ray
 *            after it has collided with the object.
 * position - the position of this hittable in 3D space.
 * object_data - pointer to arbitrary value in memory which contains properties
 *               about the object. This is specific to the type of hittable
 *               object.
 */
struct Hittable {
  struct Material *mat;
  bool (*hit_func)(struct Hittable *, const ray_t *, double, double,
                   hit_record_t *);
  vec3_t position;
  void *object_data;
};

/**
 * Frees a hittable instance from memory.
 * h must not have been freed already.
 */
void freeHittable(hittable_t *h);

/**
 * Allocates a new hittable_list to the heap.
 */
hittable_list_t *hittable_list_new();

/**
 * Adds a memory-allocated hittable to a hittable_list.
 */
void hittable_list_add(hittable_list_t *hl, hittable_t *h);

/**
 * Frees a given hittable_list, as well as all the objects inside it.
 */
void hittable_list_destroy(hittable_list_t *hl);

/**
 * Sets the collision normal of a hit_record, given the collided ray and the
 * outward normal of the hittable object it has collided with.
 */
void setFaceNormal(hit_record_t *rec, const ray_t *r,
                   const vec3_t outward_normal);

#endif  // __HITTABLE_H__

#ifndef __SPHERE__
#define __SPHERE__

/**
 * A sphere, which can be hit by rays.
 */
typedef struct Hittable sphere;

/**
 * Data specific to a sphere.
 *
 * center - the center of the sphere
 * radius - the radius of the sphere
 */
typedef struct sphere_data_t {
  double radius;
} sphere_data;

/**
 * Allocates a new sphere instance and sphere_data instance on the heap.
 * Returns NULL if the either allocation fails.
 *
 * center - the center of the new sphere
 * radius - the radius of the new sphere
 * mat - the material of the sphere. Must not be NULL.
 */
sphere *newSphere(vec3_t center, double radius, material_t *mat);

/**
 * Frees the given sphere and its associated sphere_data from memory.
 *
 * The sphere's material is NOT de-allocated.
 */
void freeSphere(sphere *s);

#endif  // __SPHERE__