#include "../include/animation.h"

animlist_t *animlist_new() { return list_new(); }

void animation_free(animation_t *animation) {
  free(animation->animation_data);
  free(animation);
}

void animlist_free(animlist_t *animlist) {
  list_destroy(animlist, (val_free_func)&animation_free);
}

void animlist_animate(animlist_t *animlist, double time) {
  double end_time;
  for (animation_t *a = (animation_t *)list_iterate(animlist); a != NULL;
       a = (animation_t *)list_iterate(NULL)) {
    end_time = a->delay + a->duration;
    if (time >= a->delay && !a->finished) {
      if (a->duration == 0) {
        a->animate_func(a, 1);
      } else {
        a->animate_func(a, pow(clamp((time - a->delay) / a->duration, 0.0, 1.0),
                               a->interp));
      }
      if (time >= end_time) {
        a->finished = true;
      }
    }
  }
}

typedef struct VaData {
  double from;
  double to;
} va_data_t;

void va_animate(animation_t *animation, double time) {
  va_t *va = (va_t *)animation;
  va_data_t *va_data = va->animation_data;
  *(double *)(va->animated_property) =
      va_data->from + (va_data->to - va_data->from) * time;
}

va_t *va_new(animlist_t *animlist, double *value, double from, double to,
             double delay, double duration, interpolation interp) {
  va_t *va = malloc(sizeof(animation_t));
  va->animate_func = &va_animate;
  va->finished = false;
  va->animated_property = value;
  va->delay = delay;
  va->duration = duration;
  va->interp = interp;
  va_data_t *va_data = malloc(sizeof(va_data_t));
  va_data->from = from;
  va_data->to = to;
  va->animation_data = va_data;
  return va;
}

typedef struct OrbitData {
  vec3_t *pivot;
  vec3_t x, y;
  double angle;
} orbit_data_t;

void orbit_animate(animation_t *orbit_animation, double time) {
  orbitanim_t *oa = (orbitanim_t *)orbit_animation;
  vec3_t *target = (vec3_t *)orbit_animation->animated_property;
  orbit_data_t *oa_data = (orbit_data_t *)oa->animation_data;
  vec3_t *pivot = oa_data->pivot;
  double current_angle = time * oa_data->angle;
  vec3_t x = vec3_mul(oa_data->x, cos(current_angle));
  vec3_t y = vec3_mul(oa_data->y, sin(current_angle));
  *target = vec3_add(*pivot, vec3_add(x, y));
}

orbitanim_t *orbitanim_new(animlist_t *animlist, vec3_t *target, vec3_t *pivot,
                           vec3_t y, double angle, double delay,
                           double duration, interpolation interp) {
  orbitanim_t *oa = malloc(sizeof(orbitanim_t));
  oa->animate_func = &orbit_animate;
  oa->finished = false;
  oa->animated_property = target;
  oa->delay = delay;
  oa->duration = duration;
  oa->interp = interp;
  orbit_data_t *oa_data = malloc(sizeof(orbit_data_t));
  oa_data->pivot = pivot;
  oa_data->x = vec3_sub(*target, *pivot);
  oa_data->y = y;
  oa_data->angle = angle;
  oa->animation_data = oa_data;
  list_add(animlist, oa);
  return oa;
}
