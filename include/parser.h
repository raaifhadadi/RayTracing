
#ifndef __PARSER_H_
#define __PARSER_H_

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "animation.h"
#include "scene.h"
#include "vec3.h"
#include "colour.h"
#include "hittable.h"
#include "material.h"
#include "utils.h"
#include "hashtable.h"

/**
 * Creates a scene based on the contents of the scene specification file.
 *
 * File format:
 *
 * render ([output...]) (resolution [x] [y]) (samples [s]) (frames [f]) (fps
 * [p])
 * - Defines several properties about the render result. Line must be present.
 * - [output...] can be either "image" or "video" or both, in either order.
 *   Specifying "image" will write all the frames in the rendered animation to
 *   individual .ppm files.
 *   Specifying "video" will collate all frames into an .avi video but will not
 *   write any .ppm files.
 *   Specifying "image video" will produce both .ppm files and an .avi video.
 *   Default: video
 * - the output resolution will be [x] pixels wide and [y] pixels tall.
 *   Default: [x] = 400, [y] = 225
 * - each frame will compute [s] samples.
 *   Default: 100
 * - the output will have [f] frames.
 *   Default: 1
 * - the output will have [p] frames per second.
 *   Default: 24
 *
 * camera from [x] [y] [z] to [x] [y] [z] up [x] [y] [z] (fov [f]) (focus_dist
 * [f]) (aperture [a])
 * - Defines the camera to use for rendering. Line must be present.
 * - Camera is positioned at from. Must be specififed.
 * - Camera is pointed in direction to. Must be specified.
 * - The "up" direction on the viewport is given by the up vector. Must be
 *   specified.
 * - fov is the field of view of the camera. Default is ...
 * - focus_dist specifies the distance from the camerea to the point of focus.
 *   Default is ...
 * - aperture default is ...
 *
 * material [name] [type] (albedo [r] [g] [b]) (ir [i]) (fuzz [f])
 * - Defines a new material. A default material named "default" will always be
 *   created by the program, which will be a lambertian material with albedo
 *   (255, 255, 255). Material definitions are optional.
 * - [name] can be any ASCII sequence which does not contain spaces
 * - [type] can be either "lambertian", "metal" or "glass".
 *   Must be specified.
 * - the colour of the material is specified by (albedo)
 *   [r], [g] and [b] can take any integer value between 0 and 255.
 *   Default: [r] = 255, [g] = 255, [b] = 255
 * - the index of refraction of the material is specified by [i].
 *   It only has an effect if [type] is "glass".
 *   Default: 0.0
 * - the fuzziness of the material is specified by (fuzz)
 *   It only has an effect if [type] is "metal".
 *   Default: 0.0
 *
 * sphere (name [name]) position [x] [y] [z] (radius [r]) (material [m])
 * - Creates a new sphere in the scene. Sphere definitions are optional.
 * - [name] can be any ASCII sequence which does not contain spaces. Optional.
 * - the center of the sphere is specified by (position). Must be specified.
 *   [x], [y] and [z] can take any decimal value.
 * - the radius of the sphere is specified by (radius).
 *   [r] can take any decimal value.
 *   Default: 1.0
 * - the material of the sphere is specified by (material).
 *   [m] can be any ASCII sequence which does not contain spaces
 *   Default: default
 *
 * animate orbit around [name] y [x] [y] [z] (angle [a]) (delay [de]) (duration
 * [du]) (interpolation [i])
 * - Creates a new oribt animation in the scene name
 * - [name] must be an ASCII sequence with no spaces, must be name previously
 *   assigned to
 * - a hittable vector [y] is the height of the orbit in local 2D space.
 *   Determines direction and eccentricty [x], [y] and [z] can take any decimal
 *   value.
 * - how far around the centre the object will rotate is specified by (angle)
 *   [a] can take any decimal value.
 *   Default 360
 * - In seconds, (delay) determines how long the animation takes to start
 *   [de] can take any positive decimal value.
 *   Default 0.0
 * - In seconds, (duration) determines how long the animation takes to complete
 *   [du] can take any positive decimal value.
 *   Default 2.0
 * - (interpolation) determines the type of interpolation used.
 *   [i] must be any of: "linear", "quadratic", "cubic", "quadratic" or
 *   "quintic". Default linear
 *
 * background [style] (colour [r] [g] [b]) (top [r] [g] [b]) (bottom [r] [g]
 * [b])
 * - sets background colours
 * - [style] must be "gradient" or "plain"
 * - [r], [g] and [b] can take any integer value between 0 and 255.
 * - if [style] is "plain", (only) colour must be provided
 * - if [style] is "gradient", (only) top and bottom must be provided
 */

void readScene(render_t *render_info, scene_t *sc, FILE *source);

// For testing
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

#endif  // __PARSER_H_
