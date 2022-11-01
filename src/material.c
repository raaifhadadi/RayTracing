#include "../include/material.h"

#include <math.h>

bool scatter_lmb(const material_t *mat, const ray_t r_in,
                 const hit_record_t hit_rec, colour_t *attenuation,
                 ray_t *scattered) {
  vec3_t scattered_dir = vec3_add(hit_rec.normal, vec3_randUnit());
  if (vec3_close_to_zero(scattered_dir)) {
    scattered_dir = hit_rec.normal;
  }
  scattered->origin = hit_rec.point;
  scattered->direction = scattered_dir;
  *attenuation = mat->albedo;
  return true;
}

bool scatter_metal(const material_t *mat, const ray_t r_in,
                   const hit_record_t hit_rec, colour_t *attenuation,
                   ray_t *scattered) {
  vec3_t reflected_dir = vec3_reflect(r_in.direction, hit_rec.normal);
  scattered->origin = hit_rec.point;
  vec3_t fuzz_vec = vec3_mul(vec3_randInSphere(), mat->fuzz);
  scattered->direction = vec3_add(reflected_dir, fuzz_vec);
  *attenuation = mat->albedo;
  return (vec3_dot(scattered->direction, hit_rec.normal) > 0);
}

double schlick_approximation(double cos, double ir) {
  double r0 = (1 - ir) / (1 + ir);
  r0 *= r0;
  return r0 + (1 - r0) * pow((1 - cos), 5);
}

bool scatter_glass(const material_t *mat, const ray_t r_in,
                   const hit_record_t hit_rec, colour_t *attenuation,
                   ray_t *scattered) {
  *attenuation = mat->albedo;
  double etaRatio = hit_rec.front_face ? (1.0 / mat->ir) : mat->ir;
  vec3_t dir = vec3_normalize(r_in.direction);
  double cos_theta = fmin(vec3_dot(vec3_mul(dir, -1), hit_rec.normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
  scattered->origin = hit_rec.point;
  if (etaRatio * sin_theta > 1.0 ||
      schlick_approximation(cos_theta, mat->ir) > rand_double()) {
    scattered->direction = vec3_reflect(dir, hit_rec.normal);
  } else {
    scattered->direction = vec3_refract(dir, hit_rec.normal, etaRatio);
  }
  return true;
}

material_t *create_metal(scene_t *scene, colour_t col, double fuzziness) {
  material_t *mat = malloc(sizeof(material_t));
  mat->albedo = col;
  mat->scatter = scatter_metal;
  mat->ir = 1;
  mat->fuzz = fuzziness;
  scene_addMaterial(scene, mat);
  return mat;
}

material_t *create_lmb(scene_t *scene, colour_t col) {
  material_t *mat = malloc(sizeof(material_t));
  mat->albedo = col;
  mat->scatter = scatter_lmb;
  mat->ir = 1;
  mat->fuzz = 0.0;
  scene_addMaterial(scene, mat);
  return mat;
}

material_t *create_glass(scene_t *scene, colour_t col, double index) {
  material_t *mat = malloc(sizeof(material_t));
  mat->albedo = col;
  mat->scatter = scatter_glass;
  mat->ir = index;
  mat->fuzz = 0.0;
  scene_addMaterial(scene, mat);
  return mat;
}

void free_material(material_t *mat) { free(mat); }
