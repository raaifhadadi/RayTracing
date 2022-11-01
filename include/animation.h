#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <math.h>
#include <stdbool.h>

#include "list.h"
#include "vec3.h"
#include "utils.h"

/**
 * Types of interpolation.
 */
typedef enum Interpolation {
  LINEAR = 1,
  QUADRATIC = 2,
  CUBIC = 3,
  QUARTIC = 4,
  QUINTIC = 5
} interpolation;

struct Animation;

/**
 * A function which changes a value in memory every time it is called, based on
 * the provided time.
 *
 * The time ranges from 0.0 to 1.0, where 0.0 is the start of the animation
 * (after any delays) and 1.0 is the end of the animation (at the end of its
 * duration).
 */
typedef void (*animation_func)(struct Animation *, double);

/**
 * An animation which can act on anything.
 *
 * animate_func - the function which changes animated_property each time it is
 *                called, based on the provided time.
 * animated_property - pointer to the value in memory that is being changed
 *                     by this animation instance.
 * delay - how long it takes for the animation to start, in units.
 *         animated_property should not be changed during this time.
 *         Ranges from 0.0 to 1.0; delay of 0.0 means the animation begins
 *         immediately on frame 1 and delay of 1.0 means the animation does not
 *         run at all (this shouldn't happen).
 * duration - how long the animation lasts, in units. animated_property will be
 *            changed between delay and delay + duration, and should not be
 *            changed after delay + duration.
 * interp - how the animation interpolates between the start and end states.
 * finished - true when the animation has been fully executed according to its
 *            duration.
 * animation_data - arbitrary data used by animate_func to perform the
 *                  animation.
 */
typedef struct Animation {
  animation_func animate_func;
  void *animated_property;
  double delay;
  double duration;
  interpolation interp;
  bool finished;
  void *animation_data;
} animation_t;

/**
 * A list of animations which are updated every frame.
 * There should be only one per scene.
 */
typedef list_t animlist_t;

/**
 * Allocates an animation list on the heap.
 * Returns NULL if the allocation failed.
 */
animlist_t *animlist_new();

/**
 * De-allocates an animlist, including its contents.
 */
void animlist_free(animlist_t *animlist);

/**
 * Update all the animations in the given list. Time should be between 0 and 1.
 */
void animlist_animate(animlist_t *animlist, double time);

#endif  // __ANIMATION_H__

#ifndef __VALUE_ANIM__
#define __VALUE_ANIM__

typedef animation_t va_t;

/**
 * Creates a new animation which interpolates the given value from the
 * start value to the end value.
 * Adds the resulting animation to the provided animation list.
 */
va_t *va_new(animlist_t *animlist, double *value, double from, double to,
             double delay, double duration, interpolation interp);

#endif  // __VALUE_ANIM__

#ifndef __ORBIT_ANIM__
#define __ORBIT_ANIM__

typedef animation_t orbitanim_t;

/**
 * Creates a new animation which rotates the given vec3 by an angle around a
 * pivot point in 3D space.
 * The vector "y" is the height of the orbit in local 2D space. It determines
 * both the direction of the rotation and how elliptic it is in 3D space.
 * If y is perpendicular to pivot - target, and has equal length to
 * pivot - target, the orbit will be circular.
 */
orbitanim_t *orbitanim_new(animlist_t *animlist, vec3_t *target, vec3_t *pivot,
                           vec3_t y, double angle, double delay,
                           double duration, interpolation interp);

#endif  // __ORBIT_ANIM__
