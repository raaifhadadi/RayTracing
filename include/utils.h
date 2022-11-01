#ifndef __UTILS_H__
#define __UTILS_H__

#define PI 3.1415926535897932385

#include <math.h>

#include "colour.h"

/**
 * Converts from degrees to radians.
 */
double degreesToRadians(double degrees);

/**
 * Returns a random double between 0 and 1.
 */
double rand_double();

/**
 * Returns a random double between min and max.
 */
double rand_double_range(double min, double max);

/**
 * Restricts the value x between min and max.
 *
 * If x is below min, returns min.
 * If x is above max, returns max.
 * Otherwise, returns x.
 */
double clamp(double x, double min, double max);

/**
 * Returns a colour instance whose components are random values.
 */
colour_t rand_colour(void);

/**
 * Compares the value of two doubles, with accuracy DBL_EPSILON
 */
bool double_equals(double a, double b);

#endif  // __UTILS_H__
