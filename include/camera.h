#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "vec3.h"
#include "ray.h"

/**
 * A camera.
 *
 * origin - the position of the camera
 * lower_left_corner - the position vector which corresponds to the lower left
 *                     corner of the viewport.
 * horizontal - horizontal width of the viewport in 3D space.
 * vertical - vertical width of the viewport in 3D space.
 * u, v, w - directional vectors of the camera.
 *           u corresponds to the local x axis ("left/right")
 *           v corresponds to the local z axis ("up/down")
 *           w corresponds to the local y axis ("far/close")
 * lens_radius - the radius of the camera's lens. Determines the size of the
 *               defocus blur.
 */
typedef struct Camera {
  vec3_t origin;
  vec3_t lower_left_corner;
  vec3_t horizontal;
  vec3_t vertical;
  vec3_t u, v, w;
  double lens_radius;
} camera_t;

/**
 * Initialises a camera instance.
 *
 * look_from - the 3D position of the camera
 * look_at - the 3D point which the camera is looking at
 * vup - the vector which corresponds to the up direction in the viewport
 * vfov - the vertical field of view of the camera, in degrees
 * aspect_ratio - aspect ratio of the rendered image
 * aperture - aperture of the camera lens, dictates how large the defocus blur
 *            is in the final rendered image
 * focus_dist -
 */
camera_t* camera_new(vec3_t look_from, vec3_t look_at, vec3_t vup, double vfov,
                     double aspect_ratio, double aperture, double focus_dist);

/**
 * Generates a ray.
 *
 * cam - pointer to the camera instance of the scene
 * x - horizontal position that the ray will correspond to on the viewport,
 *     given as a fraction of the viewport width. 0.0 is all the way to the
 *     left, 1.0 is all the way to the right.
 * y - vertical position that the ray will correspond to on the viewport,
 *     given as a fraction of the viewport height. 0.0 is all the way down,
 *     1.0 is all the way up.
 */
ray_t getRay(camera_t* cam, double x, double y);

#endif  // __CAMERA_H_
