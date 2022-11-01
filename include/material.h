#ifndef __MATERIAL__
#define __MATERIAL__
struct Material;
typedef struct Material material_t;
#endif  // __MATERIAL__

#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include "ray.h"
#include "colour.h"
#include "hittable.h"
#include "scene.h"
#include "vec3.h"
#include "ray.h"
#include "colour.h"
#include "utils.h"

/**
 * A physically-based rendering material.
 *
 * scatter - function which determines how a ray is scattered when it hits an
 *           object with this material
 * albedo - the colour of the material
 * ir - index of refraction of the material, if it is transmissive
 * fuzz - roughness of the material. Determines how much light is scattered by
 *        this material, if the material is "shiny".
 */
struct Material {
  bool (*scatter)(const material_t *mat, const ray_t r_in,
                  const hit_record_t hit_rec, colour_t *attenuation,
                  ray_t *scattered);
  colour_t albedo;
  double ir;
  double fuzz;
};

/**
 * Creates a new metal material.
 *
 * col - the albedo of the material
 * fuzz - the fuzziness of the metal reflection
 */
material_t *create_metal(scene_t *scene, colour_t col, double fuzz);

/**
 * Creates a new lambertian material.
 *
 * col - the albedo of the material
 */
material_t *create_lmb(scene_t *scene, colour_t col);

/**
 * Creates a new glass material.
 *
 * col - the albedo of the material
 * index - the index of refraction of the glass
 */
material_t *create_glass(scene_t *scene, colour_t col, double index);

/**
 * De-allocates the given material from memory.
 */
void free_material(material_t *mat);

#endif  // __MATERIAL_H_
