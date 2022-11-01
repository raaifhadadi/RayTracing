#include "../include/buffer.h"

#include <stdlib.h>

/*
 * Initializes a framebuffer struct, sends to the VC via mailbox 0, then awaits
 * the response
 *
 * Params: fbInfo - pointer to the framebuffer information structure
 *         width  - the requested frame width
 *         height - the requested frame height
 *         depth  - the requested bit depth
 * Side effects: the VC overwrites the fbInfo structure with the provided
 * framebuffer information Returns: none
 */
frameBuffer *frameBuffer_new(uint32_t width, uint32_t height, uint32_t depth) {
  frameBuffer *frame = malloc(sizeof(frameBuffer));
  frame->width = width;
  frame->height = height;
  frame->depth = depth;
  frame->buffer = malloc(width * height * sizeof(uint32_t));

  return frame;
}

void frameBuffer_destroy(frameBuffer *fbInfo) {
  if (fbInfo) {
    free(fbInfo->buffer);
  }
  free(fbInfo);
}

void setPixel(frameBuffer *frame, uint32_t x, uint32_t y, uint32_t color) {
  frame->buffer[y * frame->width + x] = color;
}
