#include "../include/colour.h"

#include <math.h>

#include "../include/utils.h"

uint32_t colourToInt(colour_t *c) {
  return 0xFF000000 | ((uint32_t)(c->b) << 16) | ((uint32_t)(c->g) << 8) |
         (uint32_t)(c->r);
}

void writeColour(frameBuffer *fb, uint32_t x, uint32_t y, colour_t *c) {
  setPixel(fb, x, y, colourToInt(c));
}

colour_t colour_new(double r, double b, double g) {
  return (colour_t){clamp(r * 255, 0, 255), clamp(b * 255, 0, 255),
                    clamp(g * 255, 0, 255)};
}

colour_t vec3ToColour(const vec3_t a) {
  colour_t c;
  c.r = clamp(a.x, 0, 255);
  c.g = clamp(a.y, 0, 255);
  c.b = clamp(a.z, 0, 255);
  return c;
}

vec3_t colourToVec3(const colour_t c) { return vec3_new(c.r, c.g, c.b); }