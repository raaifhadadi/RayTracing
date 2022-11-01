#ifndef __SCENE_STRUCT__
#define __SCENE_STRUCT__
struct Scene;
typedef struct Scene scene_t;
#endif  // __SCENE_STRUCT__

#ifndef __SCENE_H__
#define __SCENE_H__

#include <math.h>
#include <stdio.h>

#include "camera.h"
#include "colour.h"
#include "buffer.h"
#include "hittable.h"
#include "material.h"
#include "utils.h"
#include "animation.h"

/**
 * Background lighting for the scene.
 */
typedef struct Background {
  colour_t top;
  colour_t bottom;
} background_t;

/**
 * The raytraced scene.
 *
 * world - a list of the hittable objects in this scene
 * materials - a list of the materials used by the objects in the world
 * cam - the sole viewport camera which be used for rendering
 */
struct Scene {
  hittable_list_t *world;
  list_t *materials;
  animlist_t *animations;
  camera_t *camera;
  background_t background;
};

typedef enum OutputType { VIDEO, IMAGE } output_type_t;

/**
 * Stores information about the render, as well as the frame buffer used for
 * rendering.
 */
typedef struct Render {
  frameBuffer *buffer;
  uint32_t samples;
  uint32_t num_frames;
  uint32_t fps;
  uint32_t output;
} render_t;

/**
 * Allocates a new scene on the heap.
 * Returns NULL if the allocation fails.
 */
scene_t *scene_new();

/**
 * De-allocates a scene in its entirety, including its lists' contents.
 */
void scene_destroy(scene_t *s);

/**
 * Adds a heap-allocated material to this scene.
 */
material_t *scene_addMaterial(scene_t *s, material_t *mat);

/**
 * Adds a heap-allocation hittable object to this scene.
 */
hittable_t *scene_addHittable(scene_t *s, hittable_t *h);

/**
 * Renders the given scene.
 *
 * samples - the number of rays to cast for each pixel in the framebuffer
 */
void renderScene(scene_t *scene, frameBuffer *buffer, int samples);

/**
 * Re-creates the scene shown at the end of Peter Shirley's
 * "Ray Tracing in One Weekend" book.
 */
void bookScene(render_t *render_info, scene_t *scene);

#endif  // __SCENE_H__
