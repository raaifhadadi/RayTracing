#include "../include/buffer.h"
#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define NUM_CHARS 80

void decodeColour(const uint32_t colour, uint32_t *r, uint32_t *g,
                  uint32_t *b) {
  *r = 0x000000FF & colour;
  *g = (0x0000FF00 & colour) >> 8;
  *b = (0x00FF0000 & colour) >> 16;
}

void writeFile(const char *filename, const frameBuffer *fbInfo) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    return;
  }
  fprintf(file, "P3\n");
  fprintf(file, "%d %d\n", fbInfo->width, fbInfo->height);
  fprintf(file, "255\n");
  for (int i = 0; i < fbInfo->height; ++i) {
    for (int j = 0; j < fbInfo->width; ++j) {
      uint32_t r, g, b;
      decodeColour(fbInfo->buffer[i * fbInfo->width + j], &r, &g, &b);
      fprintf(file, "%d %d %d\n", r, g, b);
    }
  }
  fclose(file);
}

void renderAnimation(const render_t *render, scene_t *scene,
                     char file_name[NUM_CHARS]) {
  char output_name[NUM_CHARS];
  strcpy(output_name, file_name);
  int name_len = strlen(file_name);

  int frame = 1;
  double animation_time;

  printf("Render settings:\n");
  if (render->num_frames > 1) {
    printf(
        " | Width: %d, Height: %d\n"
        " | Samples: %d\n"
        " | Num. frames: %d, Framerate: %d\n"
        " | Animation length: %.3lf seconds\n",
        render->buffer->width, render->buffer->height, render->samples,
        render->num_frames, render->fps,
        ((double)render->num_frames) / ((double)render->fps));
  } else {
    printf(
        " | Width: %d, Height: %d\n"
        " | Samples: %d\n",
        render->buffer->width, render->buffer->height, render->samples);
  }

  printf("Begin rendering...\n");

  clock_t current_time;
  while (frame <= render->num_frames) {
    printf(" | Rendering frame %d of %d... ", frame, render->num_frames);
    current_time = clock();
    animation_time = ((double)frame) / (double)render->num_frames;
    animlist_animate(scene->animations, animation_time);
    renderScene(scene, render->buffer, render->samples);
    if (render->num_frames > 0) sprintf(output_name + name_len, "_%03d", frame);
    sprintf(output_name + strlen(output_name), ".ppm");
    writeFile(output_name, render->buffer);
    printf("Done in %.3f seconds\n",
           (double)(clock() - current_time) / CLOCKS_PER_SEC);
    frame++;
  }

  printf("Rendering finished!\n");
}

int main(int argc, char **argv) {
  setbuf(stdout, NULL);

  time_t t;
  srand((unsigned)time(&t));

  char file_name[NUM_CHARS];
  strcpy(file_name, "output");

  scene_t *scene = scene_new();
  render_t *render_info = malloc(sizeof(render_t));

  if (scene == NULL) exit(1);
  if (render_info == NULL) exit(1);

  if (argc == 2) {
    printf("Loading scene \"%s\"...\n", argv[1]);
    strcpy(file_name, argv[1]);
    FILE *f = fopen(file_name, "r");
    readScene(render_info, scene, f);
    fclose(f);
  } else {
    printf("Loading default scene...\n");
    bookScene(render_info, scene);
  }

  renderAnimation(render_info, scene, file_name);

  scene_destroy(scene);
  frameBuffer_destroy(render_info->buffer);
  free(render_info);
}
