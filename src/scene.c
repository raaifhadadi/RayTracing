#include "../include/scene.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define CONCURRENT false
#define NUM_THREADS 8

#define NUM_SAMPLES 10
#define MAX_DEPTH 5
#define EPSILON 0.0000001

scene_t *scene_new() {
  scene_t *s = malloc(sizeof(scene_t));
  if (s == NULL) return NULL;
  s->world = hittable_list_new();
  if (s->world == NULL) return NULL;
  s->materials = list_new();
  if (s->materials == NULL) return NULL;
  s->animations = list_new();
  if (s->animations == NULL) return NULL;
  s->background.top = colour_new(128, 179, 255);
  s->background.bottom = colour_new(255, 255, 255);
  return s;
}

material_t *scene_addMaterial(scene_t *s, material_t *mat) {
  list_add(s->materials, mat);
  return mat;
}

hittable_t *scene_addHittable(scene_t *s, hittable_t *h) {
  hittable_list_add(s->world, h);
  return h;
}

void scene_destroy(scene_t *s) {
  hittable_list_destroy(s->world);
  list_destroy(s->materials, (val_free_func)&free_material);
  animlist_free(s->animations);
  free(s->camera);
  free(s);
}

vec3_t ray_colour(const ray_t *r, hittable_list_t *world, int depth,
                  const background_t *back) {
  if (depth <= 0) {
    return (vec3_t){0, 0, 0};
  }
  hit_record_t rec;
  if (world->hit_func(world, r, EPSILON, INFINITY, &rec)) {
    ray_t scattered;
    colour_t attenuation;
    if (rec.mat->scatter(rec.mat, *r, rec, &attenuation, &scattered)) {
      vec3_t raycol = ray_colour(&scattered, world, depth - 1, back);
      return vec3_new(((double)attenuation.r) / 255.0 * raycol.x,
                      ((double)attenuation.g) / 255.0 * raycol.y,
                      ((double)attenuation.b) / 255.0 * raycol.z);
    }
    return (vec3_t){0.0, 0.0, 0.0};
  }
  double t = 0.5 * (vec3_normalize(r->direction).y + 1.0);
  vec3_t top = colourToVec3(back->top);
  vec3_t bottom = colourToVec3(back->bottom);
  top = vec3_mul(top, t / 256.0);
  bottom = vec3_mul(bottom, (1.0 - t) / 256.0);
  return vec3_add(top, bottom);
}

void bookScene(render_t *render_info, scene_t *scene) {
  render_info->buffer = frameBuffer_new(400, 225, 100);
  render_info->num_frames = 1;
  render_info->fps = 0;
  render_info->samples = 50;

  double aspect_ratio = ((double)render_info->buffer->width) /
                        ((double)render_info->buffer->height);
  vec3_t look_from = vec3_new(13.0, 2.0, 3.0);
  vec3_t look_at = vec3_new(0.0, 0.0, 0.0);
  double vfov = 20;
  vec3_t vup = vec3_new(0.0, 1.0, 0.0);
  double dist_to_focus = 10.0;
  double aperture = 0.1;
  scene->camera = camera_new(look_from, look_at, vup, vfov, aspect_ratio,
                             aperture, dist_to_focus);

  material_t *mat_ground = create_lmb(scene, colour_new(0.5, 0.5, 0.5));
  scene_addHittable(scene,
                    newSphere(vec3_new(0.0, -1000, 0.0), 1000.0, mat_ground));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      double choose_mat = rand_double();
      vec3_t center = {a + 0.9 * rand_double(), 0.2, b + 0.9 * rand_double()};
      vec3_t diff = vec3_sub(center, vec3_new(4, 0.2, 0));

      material_t *mat;

      if (vec3_length(diff) > 0.9) {
        if (choose_mat < 0.8) {
          // diffuse
          mat = create_lmb(scene, rand_colour());
        } else if (choose_mat < 0.95) {
          // metal
          mat = create_metal(scene, rand_colour(), rand_double_range(0, 0.5));
        } else {
          // glass
          mat = create_glass(scene, colour_new(1.0, 1.0, 1.0), 1.5);
        }
      }

      scene_addHittable(scene, newSphere(center, 0.2, mat));
    }
  }

  material_t *mat1 = create_glass(scene, colour_new(1.0, 1.0, 1.0), 1.5);
  scene_addHittable(scene, newSphere(vec3_new(0, 1, 0), 1.0, mat1));

  material_t *mat2 = create_lmb(scene, colour_new(0.4, 0.2, 0.1));
  scene_addHittable(scene, newSphere(vec3_new(-4, 1, 0), 1.0, mat2));

  material_t *mat3 = create_metal(scene, colour_new(0.7, 0.6, 0.5), 0.0);
  scene_addHittable(scene, newSphere(vec3_new(4.0, 1.0, 0.0), 1.0, mat3));
}

typedef struct thread_data_t {
  int id;
  int start;
  int end;
  int samples;
  frameBuffer *buffer;
  scene_t *scene;
} thread_data_t;

void *renderWorker(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;

  for (int i = data->start; i < data->end; ++i) {
    for (int j = 0; j < data->buffer->width; ++j) {
      vec3_t c = {0, 0, 0};
      for (int s = 0; s < data->samples; ++s) {
        double u = (rand_double() + j) / (data->buffer->width - 1);
        double v = (rand_double() + i) / (data->buffer->height - 1);
        ray_t r = getRay(data->scene->camera, u, v);
        vec3_t temp = ray_colour(&r, data->scene->world, MAX_DEPTH,
                                 &data->scene->background);
        c = vec3_add(c, temp);
      }
      c = vec3_div(c, data->samples);
      c = vec3_apply_func(c, sqrt);
      c = vec3_mul(c, 256.0);
      colour_t result = vec3ToColour(c);

      writeColour(data->buffer, j, data->buffer->height - i - 1, &result);
    }
  }

  pthread_exit(NULL);
}

void concurrentRenderScene(scene_t *scene, frameBuffer *buffer, int samples) {
  if (scene == NULL) return;
  if (buffer == NULL) return;
  const int smp = samples != 0 ? samples : NUM_SAMPLES;

  int nThreads = CONCURRENT ? NUM_THREADS : 1;
  int block = buffer->height / nThreads;

  if (buffer->height % nThreads > 0) {
    nThreads++;
  }

  pthread_t worker[nThreads];
  thread_data_t data[nThreads];
  int i, rc;

  for (i = 0; i < nThreads; i++) {
    data[i].id = i;
    data[i].start = i * block;
    data[i].end = fmin((i + 1) * block, buffer->height);
    data[i].samples = smp;
    data[i].buffer = buffer;
    data[i].scene = scene;

    printf("\n | Thread: %d | Start: %d | End: %d\n", data[i].id, data[i].start,
           data[i].end);
    if ((rc = pthread_create(&worker[i], NULL, renderWorker, &data[i]))) {
      fprintf(stderr, "Error creating thread: %d\n", rc);
    }
  }

  for (i = 0; i < nThreads; ++i) {
    pthread_join(worker[i], NULL);
  }
}

void renderScene(scene_t *scene, frameBuffer *buffer, int samples) {
  if (scene == NULL) return;
  if (buffer == NULL) return;
  const int smp = samples != 0 ? samples : NUM_SAMPLES;
  for (int i = 0; i < buffer->height; ++i) {
    for (int j = 0; j < buffer->width; ++j) {
      vec3_t c = {0, 0, 0};
      for (int s = 0; s < smp; ++s) {
        double u = (rand_double() + j) / (buffer->width - 1);
        double v = (rand_double() + i) / (buffer->height - 1);
        ray_t r = getRay(scene->camera, u, v);
        vec3_t temp =
            ray_colour(&r, scene->world, MAX_DEPTH, &scene->background);
        c = vec3_add(c, temp);
      }
      c = vec3_div(c, smp);
      c = vec3_apply_func(c, sqrt);
      c = vec3_mul(c, 256.0);
      colour_t result = vec3ToColour(c);
      writeColour(buffer, j, buffer->height - i - 1, &result);
    }
  }
}
