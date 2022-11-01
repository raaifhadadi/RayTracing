#ifndef __COLOUR_H__
#define __COLOUR_H__

#include "colour.h"
#include "buffer.h"
#include "vec3.h"

/**
 * A 32-bit colour, without alpha.
 */
typedef struct Colour {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} colour_t;

/**
 * Create a new colour instance.
 */
colour_t colour_new(double r, double b, double g);

/**
 * Converts a 32-bit colour instance to a 32-bit integer.
 */
uint32_t colourToInt(colour_t *c);

/**
 * Converts a vec3 instance to a new colour instance, where the x, y, z
 * components of the vector correspond to the r, g, b values of the colour
 * respectively.
 */
colour_t vec3ToColour(const vec3_t a);

/**
 * Converts a colour to a vec3 instance.
 */
vec3_t colourToVec3(const colour_t c);

/**
 * Writes a colour to a framebuffer instance.
 *
 * x - the x position of the pixel to write, from the left
 * y - the y position of the pixel to write, from the bottom
 * c - the colour instance in question
 */
void writeColour(frameBuffer *fb, uint32_t x, uint32_t y, colour_t *c);

#endif  // __COLOUR_H__
