#include "../include/utils.h"
#include <assert.h>

void testDegreesToRadians() {
  assert(double_equals(degreesToRadians(0), 0));
  assert(double_equals(degreesToRadians(90), PI / 2));
  assert(double_equals(degreesToRadians(180), PI));
  assert(double_equals(degreesToRadians(360), 2 * PI));
}

void testClamp() {
  assert(double_equals(clamp(0.5, 0, 1), 0.5));
  assert(double_equals(clamp(-0.5, 0, 1), 0));
  assert(double_equals(clamp(1.5, 0, 1), 1));
}

int main(void) {
  testDegreesToRadians();
  testClamp();
  return 0;
}
