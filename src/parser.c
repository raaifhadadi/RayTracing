#include "../include/parser.h"
#include "../include/buffer.h"

#define MAX_LINE_SIZE 256

char *parseVec(vec3_t *v, char *line);

char *parseAlbedo(colour_t *col, char *line);

void parseRender(render_t *render_info, char *line);

void parseCamera(camera_t **cam, char *line);

void parseMaterial(scene_t *sc, hashtable_t *materials, char *line);

void parseSphere(scene_t *sc, hashtable_t *hittables, hashtable_t *materials,
                 char *line, hittable_t **prev);

void parseOrbit(scene_t *sc, const render_t *render_info,
                hashtable_t *hittables, hittable_t *prev, char *line);

void parseBackgroundGradient(scene_t *sc, char *line);

void parseBackgroundPlain(scene_t *sc, char *line);

void readScene(render_t *render_info, scene_t *sc, FILE *source) {
  char line[MAX_LINE_SIZE];
  hashtable_t *hittables = createHashtable(0.75f, 10, &djb2);
  hashtable_t *materials = createHashtable(0.75f, 10, &djb2);
  hittable_t *prev = NULL;
  while (fgets(line, MAX_LINE_SIZE, source)) {
    if (strlen(line) >= 2 && line[0] == '/' && line[1] == '/') {
      continue;
    }
    char *parseType = strtok(line, " ");
    if (strcmp(parseType, "render") == 0) {
      parseRender(render_info, line);
    } else if (strcmp(parseType, "camera") == 0) {
      parseCamera(&sc->camera, line);
    } else if (strcmp(parseType, "material") == 0) {
      parseMaterial(sc, materials, line);
    } else if (strcmp(parseType, "sphere") == 0) {
      parseSphere(sc, hittables, materials, line, &prev);
    } else if (strcmp(parseType, "animate") == 0) {
      char *a_type = strtok(NULL, " ");
      if (strcmp(a_type, "orbit") == 0 && prev != NULL) {
        parseOrbit(sc, render_info, hittables, prev, line);
      }
    } else if (strcmp(parseType, "background") == 0) {
      char *b_type = strtok(NULL, " ");
      if (strcmp(b_type, "gradient") == 0) {
        parseBackgroundGradient(sc, line);
      } else if (strcmp(b_type, "plain")) {
        parseBackgroundPlain(sc, line);
      }
    }
  }
  hittables->free(hittables);
  materials->free(materials);
}

char *parseVec(vec3_t *v, char *token) {
  v->x = strtod(token, NULL);
  token = strtok(NULL, " ");
  v->y = strtod(token, NULL);
  token = strtok(NULL, " ");
  v->z = strtod(token, NULL);
  return strtok(NULL, " ");
}

char *parseAlbedo(colour_t *col, char *token) {
  col->r = strtod(token, NULL);
  token = strtok(NULL, " ");
  col->g = strtod(token, NULL);
  token = strtok(NULL, " ");
  col->b = strtod(token, NULL);
  return strtok(NULL, " ");
}

void parseRender(render_t *render_info, char *line) {
  char *argType = strtok(NULL, " ");
  uint32_t width = 400, height = 225, depth = 32, fps = 24, num_frames = 1,
           samples = 100, output = 0;
  while (argType != NULL) {
    char *arg = strtok(NULL, " ");
    if (strcmp(argType, "output") == 0) {
      if (strcmp(arg, "image") == 0) {
        output |= IMAGE;
      } else {
        output |= VIDEO;
      }
      char *next = strtok(NULL, " ");
      if (strcmp(next, "image") == 0) {
        output |= IMAGE;
      } else if (strcmp(next, "video") == 0) {
        output |= VIDEO;
      } else {
        argType = next;
        continue;
      }
    } else if (strcmp(argType, "resolution") == 0) {
      width = strtod(arg, NULL);
      arg = strtok(NULL, " ");
      height = strtod(arg, NULL);
    } else if (strcmp(argType, "samples") == 0) {
      samples = strtod(arg, NULL);
    } else if (strcmp(argType, "frames") == 0) {
      num_frames = strtod(arg, NULL);
    } else if (strcmp(argType, "fps") == 0) {
      fps = strtod(arg, NULL);
    }
    argType = strtok(NULL, " ");
  }
  if (!output) {
    output |= VIDEO;
  }
  render_info->output = output;
  render_info->buffer = frameBuffer_new(width, height, depth);
  render_info->fps = fps;
  render_info->num_frames = num_frames;
  render_info->samples = samples;
}

void parseCamera(camera_t **cam, char *line) {
  vec3_t from;
  vec3_t to;
  vec3_t up;
  strtok(NULL, " ");
  line = strtok(NULL, " ");
  line = parseVec(&from, line);
  line = strtok(NULL, " ");
  line = parseVec(&to, line);
  line = strtok(NULL, " ");
  line = parseVec(&up, line);

  // optional args
  double fov = 20;                  // default fov
  double focus_dist = 10.0;         // default dist
  double aperture = 0.1;            // default ap
  double aspect_ratio = 3.0 / 2.0;  // default asp_ratio
  char *argType = line;
  while (argType != NULL) {
    char *arg = strtok(NULL, " ");
    if (strcmp(argType, "fov") == 0) {
      fov = strtod(arg, NULL);
    } else if (strcmp(argType, "focus_dist") == 0) {
      focus_dist = strtod(arg, NULL);
    } else if (strcmp(argType, "aperture") == 0) {
      aperture = strtod(arg, NULL);
    } else if (strcmp(argType, "aspect_ratio") == 0) {
      aspect_ratio = strtod(arg, NULL);
    }
    argType = strtok(NULL, " ");
  }

  *cam = camera_new(from, to, up, fov, aspect_ratio, aperture, focus_dist);
}

void parseMaterial(scene_t *sc, hashtable_t *materials, char *line) {
  // parse mandatory inputs
  char *mat_name = strtok(NULL, " ");
  char *mat_type = strtok(NULL, " ");

  double ir = 1;
  double fuzz = 0;
  colour_t mat_albedo = {.r = 255, .g = 255, .b = 255};

  char *argType = strtok(NULL, " ");
  if (argType == NULL) {
    mat_type = strtok(mat_type, "\n");
  }
  while (argType != NULL) {
    char *arg = strtok(NULL, " ");
    if (strcmp(argType, "albedo") == 0) {
      argType = parseAlbedo(&mat_albedo, arg);
      continue;
    } else if (strcmp(argType, "ir") == 0) {
      ir = strtod(arg, NULL);
    } else if (strcmp(argType, "fuzz") == 0) {
      fuzz = strtod(arg, NULL);
      break;
    }
    argType = strtok(NULL, " ");
  }

  // create material based on mat_type
  material_t *mat;
  if (strcmp(mat_type, "metal") == 0) {
    mat = create_metal(sc, mat_albedo, fuzz);
  } else if (strcmp(mat_type, "lambertian") == 0) {
    mat = create_lmb(sc, mat_albedo);
  } else if (strcmp(mat_type, "glass") == 0) {
    mat = create_glass(sc, mat_albedo, ir);
  }

  // Add material to hashtable using its name as key
  materials->add(materials, mat_name, &mat, sizeof(material_t *));
}

void parseSphere(scene_t *sc, hashtable_t *hittables, hashtable_t *materials,
                 char *line, hittable_t **prev) {
  // mandatory args
  line = strtok(NULL, " ");
  char *name = NULL;
  if (strcmp(line, "name") == 0) {
    name = strtok(NULL, " ");
    line = strtok(NULL, " ");
  }
  line = strtok(NULL, " ");
  vec3_t pos;
  line = parseVec(&pos, line);

  // optional args
  double radius = 1.0;
  char *material_name = "default";
  char *argType = line;
  while (argType != NULL) {
    char *arg = strtok(NULL, " ");
    if (strcmp(argType, "radius") == 0) {
      radius = strtod(arg, NULL);
    } else if (strcmp(argType, "material") == 0) {
      material_name = strtok(arg, "\n");
      break;
    }
    argType = strtok(NULL, " ");
  }

  // create the sphere
  material_t *m = *((material_t **)materials->get(materials, material_name));
  sphere *s = newSphere(pos, radius, m);

  // add sphere to hashtable
  if (name != NULL) {
    hittables->add(hittables, name, &s, sizeof(sphere *));
    // s = hittables->get(hittables, name);
  }

  // add sphere to scene
  scene_addHittable(sc, s);

  // set previous object reference to sphere
  *prev = s;
}

void parseOrbit(scene_t *sc, const render_t *render_info,
                hashtable_t *hittables, hittable_t *prev, char *line) {
  // mandatory args
  line = strtok(NULL, " ");
  char *around = strtok(NULL, " ");
  line = strtok(NULL, " ");
  line = strtok(NULL, " ");
  vec3_t y;
  line = parseVec(&y, line);

  // optional args
  double angle = 360.0;
  double delay = 0.0;
  double duration = -1;
  interpolation interp = LINEAR;

  char *argType = line;
  while (argType != NULL) {
    char *arg = strtok(NULL, " ");
    if (strcmp(argType, "angle") == 0) {
      angle = strtod(arg, NULL);
    } else if (strcmp(argType, "delay") == 0) {
      delay = strtod(arg, NULL);
    } else if (strcmp(argType, "duration") == 0) {
      duration = strtod(arg, NULL);
    } else if (strcmp(argType, "interpolation") == 0) {
      // char *i_type = strtok(line, "\n");
      if (strcmp(arg, "linear") == 0) {
        interp = LINEAR;
      } else if (strcmp(arg, "quadratic") == 0) {
        interp = QUADRATIC;
      } else if (strcmp(arg, "cubic") == 0) {
        interp = CUBIC;
      } else if (strcmp(arg, "quartic") == 0) {
        interp = QUARTIC;
      } else if (strcmp(arg, "quintic") == 0) {
        interp = QUINTIC;
      }
    }
    argType = strtok(NULL, " ");
  }

  // creating the animation
  vec3_t *target = &prev->position;
  vec3_t *pivt =
      &(*((hittable_t **)hittables->get(hittables, around)))->position;
  double length =
      (double)(render_info->num_frames) / (double)(render_info->fps);
  orbitanim_new(sc->animations, target, pivt, y, degreesToRadians(angle),
                delay / length, duration == -1 ? 1.0 : duration / length,
                interp);
}

void parseBackgroundGradient(scene_t *sc, char *line) {
  colour_t top_col;
  colour_t bottom_col;
  line = strtok(NULL, " ");
  line = strtok(NULL, " ");
  line = parseAlbedo(&top_col, line);
  line = strtok(NULL, " ");
  line = parseAlbedo(&bottom_col, line);
  sc->background.top = top_col;
  sc->background.bottom = bottom_col;
}

void parseBackgroundPlain(scene_t *sc, char *line) {
  colour_t col;
  line = strtok(NULL, " ");
  line = strtok(NULL, " ");
  line = parseAlbedo(&col, line);
  sc->background.top = col;
  sc->background.bottom = col;
}
