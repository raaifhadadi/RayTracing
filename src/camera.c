#include "../include/camera.h"
#include "../include/utils.h"

#include <stdlib.h>
#include <math.h>

camera_t *camera_new(vec3_t look_from, vec3_t look_at, vec3_t vup, double vfov,
                     double aspect_ratio, double aperture, double focus_dist) {
  camera_t *cam = malloc(sizeof(camera_t));
  double theta = degreesToRadians(vfov);
  double h = tan(theta / 2.0);
  double viewport_height = 2.0 * h;
  double viewport_width = aspect_ratio * viewport_height;

  vec3_t w = vec3_normalize(vec3_sub(look_from, look_at));
  vec3_t u = vec3_normalize(vec3_cross(vup, w));
  vec3_t v = vec3_cross(w, u);

  cam->w = w;
  cam->u = u;
  cam->v = v;

  cam->origin = look_from;
  cam->horizontal = vec3_mul(u, viewport_width * focus_dist);
  cam->vertical = vec3_mul(v, viewport_height * focus_dist);

  vec3_t llc = vec3_sub(look_from, vec3_mul(w, focus_dist));
  llc = vec3_sub(llc, vec3_div(cam->horizontal, 2.0));
  cam->lower_left_corner = vec3_sub(llc, vec3_div(cam->vertical, 2.0));

  cam->lens_radius = aperture / 2.0;
  return cam;
}

ray_t getRay(camera_t *cam, double x, double y) {
  ray_t r;
  vec3_t rd = vec3_mul(vec3_randInDisc(), cam->lens_radius);
  vec3_t offset = vec3_add(vec3_mul(cam->u, rd.x), vec3_mul(cam->v, rd.y));
  r.origin = vec3_add(cam->origin, offset);
  r.direction = vec3_sub(cam->lower_left_corner, r.origin);
  r.direction = vec3_add(r.direction, vec3_mul(cam->horizontal, x));
  r.direction = vec3_add(r.direction, vec3_mul(cam->vertical, y));
  return r;
}
