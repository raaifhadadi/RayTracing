#include "../include/parser.h"
#include "../include/list.h"
#include <assert.h>

bool colour_equals(colour_t a, colour_t b) {
  return a.r == b.r && a.g == b.g && a.b == b.b;
}

void testParseVec(void) {
  char *input = "1 2 3";
  char line[6];
  strcpy(line, input);
  strtok(line, " ");
  vec3_t v;
  parseVec(&v, line);
  vec3_t vSol = (vec3_t){1, 2, 3};
  assert(vec3_equals(v, vSol));
}

void testParseAlbedo(void) {
  char *input = "0 100 255";
  char line[32];
  strcpy(line, input);
  strtok(line, " ");
  colour_t c;
  parseAlbedo(&c, line);
  colour_t k = (colour_t){0, 100, 255};
  assert(colour_equals(c, k));
}

void testParseCamera(void) {
  char *input =
      "camera from 89.2 43.5 11.1 to 0.0 0.0 0.0 up -0.1 0.0 1.1"
      "fov 39.6 focus_dist 99.9 aperture 0.1\n";
  char line[256];
  strcpy(line, input);
  strtok(line, " ");
  camera_t *cam;
  parseCamera(&cam, line);
  vec3_t look_from = (vec3_t){89.2, 43.5, 11.1};
  vec3_t look_at = (vec3_t){0, 0, 0};
  vec3_t vup = (vec3_t){-0.1, 0.0, 1.1};
  double vfov = 39.6;
  double aspect_ratio = 3.0 / 2.0;
  double aperture = 0.1;
  double focus_dist = 99.9;
  camera_t *sol = camera_new(look_from, look_at, vup, vfov, aspect_ratio,
                             aperture, focus_dist);
  assert(vec3_equals(cam->origin, sol->origin));
  assert(vec3_equals(cam->lower_left_corner, sol->lower_left_corner));
  assert(vec3_equals(cam->horizontal, sol->horizontal));
  assert(vec3_equals(cam->vertical, sol->vertical));
  assert(vec3_equals(cam->u, sol->u));
  assert(vec3_equals(cam->v, sol->v));
  assert(vec3_equals(cam->w, sol->w));
  assert(double_equals(cam->lens_radius, sol->lens_radius));
  free(cam);
  free(sol);
}

void testParseMaterial(void) {
  scene_t *s = scene_new();
  hashtable_t *h = createHashtable(0.75, 32, djb2);
  char *input = "material glass1 glass ir 1.450\n";
  char line[256];
  strcpy(line, input);
  strtok(line, " ");
  parseMaterial(s, h, line);
  material_t *m = (material_t *)h->get(h, "glass1");
  assert(m);
  colour_t c = (colour_t){255, 255, 255};
  assert(colour_equals(m->albedo, c));
  assert(m->fuzz == 0);
  assert(double_equals(m->ir, 1.45));

  input = "material lmb_blue lambertian albedo 119 243 255\n";
  strcpy(line, input);
  strtok(line, " ");
  parseMaterial(s, h, line);
  m = (material_t *)h->get(h, "lmb_blue");
  assert(m);
  c = (colour_t){119, 243, 255};
  assert(colour_equals(m->albedo, c));
  assert(m->fuzz == 0);
  assert(double_equals(m->ir, 1));

  input = "material metal_cobalt metal albedo 0 127 255 fuzz 0.3\n";
  strcpy(line, input);
  strtok(line, " ");
  parseMaterial(s, h, line);
  m = (material_t *)h->get(h, "metal_cobalt");
  assert(m);
  c = (colour_t){0, 127, 255};
  assert(colour_equals(m->albedo, c));
  assert(m->fuzz == 0.3);
  assert(double_equals(m->ir, 1));
  scene_destroy(s);
  h->free(h);
}

void testParseSphere(void) {
  scene_t *s = scene_new();
  hashtable_t *mats = createHashtable(0.75, 32, djb2);
  hashtable_t *hittables = createHashtable(0.75, 32, djb2);
  char *input = "material glass1 glass ir 1.450\n";
  char line[256];
  strcpy(line, input);
  strtok(line, " ");
  parseMaterial(s, mats, line);
  input = "sphere name sun position 0.0 0.0 0.0 radius 6.0 material glass1\n";
  strcpy(line, input);
  strtok(line, " ");
  hittable_t *prev = NULL;
  parseSphere(s, hittables, mats, line, &prev);
  hittable_t *h = (hittable_t *)hittables->get(hittables, "sun");
  assert(h);
  vec3_t pos = (vec3_t){0, 0, 0};
  assert(vec3_equals(h->position, pos));
  sphere_data *sd = (sphere_data *)h->object_data;
  assert(double_equals(sd->radius, 6.0));
  assert(h->mat == mats->get(mats, "glass1"));
}

void testParseRender(void) {
  render_t render_info;
  char *input = "render\n";
  char line[256];
  strcpy(line, input);
  strtok(line, " ");
  parseRender(&render_info, line);
  assert(render_info.buffer->buffer);
  assert(render_info.buffer->depth == 32);
  assert(render_info.buffer->width == 400);
  assert(render_info.buffer->height == 225);
  assert(render_info.fps == 24);
  assert(render_info.num_frames == 1);
  assert(render_info.samples == 100);
  assert(render_info.output == VIDEO);

  input =
      "render output image video resolution 1920 1080 samples 50"
      "frames 60 fps 30\n";
  strcpy(line, input);
  strtok(line, " ");
  parseRender(&render_info, line);
  assert(render_info.buffer->buffer);
  assert(render_info.buffer->depth == 32);
  assert(render_info.buffer->width == 1920);
  assert(render_info.buffer->height == 1080);
  assert(render_info.fps == 30);
  assert(render_info.num_frames == 60);
  assert(render_info.samples == 50);
  assert(render_info.output == (VIDEO | IMAGE));
}

typedef struct OrbitData {
  vec3_t *pivot;
  vec3_t last_pivot;
  vec3_t y;
  double angle;
} orbit_data_t;

void testParseOrbit(void) {
  scene_t *s = scene_new();
  render_t render_info;
  char *input = "render\n";
  char line[256];
  strcpy(line, input);
  strtok(line, " ");
  parseRender(&render_info, line);
  hashtable_t *hittables = createHashtable(0.75, 32, djb2);
  hashtable_t *mats = createHashtable(0.75, 32, djb2);
  input = "material glass1 glass ir 1.450\n";
  strcpy(line, input);
  strtok(line, " ");
  parseMaterial(s, mats, line);
  input = "sphere name sun position 0.0 0.0 0.0 radius 6.0 material glass1\n";
  strcpy(line, input);
  strtok(line, " ");
  hittable_t *prev = NULL;
  parseSphere(s, hittables, mats, line, &prev);
  input = "sphere position 4.93995 6.287 0.0 radius 0.64 material lmb_orange\n";
  strcpy(line, input);
  strtok(line, " ");
  parseSphere(s, hittables, mats, line, &prev);
  input = "animate orbit around sun y -10.0 0.0 0.0 angle 1080\n";
  strcpy(line, input);
  strtok(line, " ");
  strtok(NULL, " ");
  parseOrbit(s, &render_info, hittables, prev, line);
  animation_t *anim = s->animations->head->value;
  assert(anim);
  assert(anim->animated_property == &prev->position);
  assert(double_equals(anim->delay, 0));
  assert(double_equals(anim->duration, 1));
  assert(anim->finished == false);
  assert(anim->interp == LINEAR);
  orbit_data_t *orbAnim = anim->animation_data;
  assert(orbAnim);
  hittable_t *h = hittables->get(hittables, "sun");
  assert(h);
  assert(orbAnim->pivot == &h->position);
  vec3_t y = (vec3_t){-10.0, 0.0, 0.0};
  assert(vec3_equals(orbAnim->y, y));
  assert(double_equals(orbAnim->angle, degreesToRadians(1080)));
}

void testParseBackground() {
  scene_t *s = scene_new();
  char *input = "background plain colour 50 100 150\n";
  char line[256];
  strcpy(line, input);
  strtok(line, " ");
  strtok(NULL, " ");
  parseBackgroundPlain(s, line);
  colour_t c = (colour_t){50, 100, 150};
  assert(colour_equals(s->background.top, c));
  assert(colour_equals(s->background.bottom, c));

  input = "background gradient top 0 0 0 bottom 50 100 150\n";
  strcpy(line, input);
  strtok(line, " ");
  strtok(NULL, " ");
  parseBackgroundGradient(s, line);
  colour_t c2 = (colour_t){0, 0, 0};
  assert(colour_equals(s->background.top, c2));
  assert(colour_equals(s->background.bottom, c));
}

int main() {
  testParseVec();
  testParseAlbedo();
  testParseCamera();
  testParseMaterial();
  testParseSphere();
  testParseRender();
  testParseOrbit();
  testParseBackground();
  return 0;
}
