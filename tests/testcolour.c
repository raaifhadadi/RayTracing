#include "../include/colour.h"
#include <assert.h>

void testColourToInt(void) {
  colour_t c = (colour_t){0, 0, 0};
  assert(colourToInt(&c) == 0xFF000000);
  c = (colour_t){255, 255, 255};
  assert(colourToInt(&c) == 0xFFFFFFFF);
  c = (colour_t){255, 0, 0};
  assert(colourToInt(&c) == 0xFF0000FF);
  c = (colour_t){0, 255, 0};
  assert(colourToInt(&c) == 0xFF00FF00);
  c = (colour_t){0, 0, 255};
  assert(colourToInt(&c) == 0xFFFF0000);
}

void testVec3ToColour(void) {
  vec3_t v = (vec3_t){0, 0, 0};
  colour_t c = vec3ToColour(v);
  assert(c.r == 0 && c.g == 0 && c.b == 0);
  v.x = 50.1;
  c = vec3ToColour(v);
  assert(c.r == 50 && c.g == 0 && c.b == 0);
  v.y = 255;
  v.z = 1000;
  c = vec3ToColour(v);
  assert(c.r == 50 && c.g == 255 && c.b == 255);
}

int main(void) {
  testColourToInt();
  testVec3ToColour();
  return 0;
}
