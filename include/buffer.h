#ifndef __BUFFER_H_
#define __BUFFER_H_

#include <stdint.h>

typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t vWidth;
  uint32_t vHeight;
  uint32_t pitch;
  uint32_t depth;
  uint32_t xOffset;
  uint32_t yOffset;
  uint32_t* buffer;
  uint32_t fbSize;
} frameBuffer;

frameBuffer* frameBuffer_new(uint32_t width, uint32_t height, uint32_t depth);

void frameBuffer_destroy(frameBuffer* fbInfo);

void setPixel(frameBuffer* fbInfo, uint32_t x, uint32_t y, uint32_t color);

#endif
