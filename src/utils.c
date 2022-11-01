#include "../include/utils.h"

#include <math.h>
#include <stdlib.h>

#define DBL_EPSILON 0.000001

bool double_equals(double a, double b) { return fabs(a - b) < DBL_EPSILON; }

double degreesToRadians(double degrees) { return degrees * PI / 180.0; }

double rand_double() { return ((double)rand()) / (RAND_MAX); }

double rand_double_range(double min, double max) {
  return min + (max - min) * rand_double();
}

double clamp(double x, double min, double max) {
  double result = x < min ? min : x;
  return result > max ? max : result;
}

colour_t rand_colour(void) {
  double r = rand_double_range(0, 255);
  double g = rand_double_range(0, 255);
  double b = rand_double_range(0, 255);
  return (colour_t){r, g, b};
}
