#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "logger.h"
#include "engine.h"
#include "sprite.h"

typedef struct Point {
  float x;
  float y;
} Point;

static void print_points(Sprite *sprite)
{
  for (int i = 0; i < SPRITE_NUM_ATTRIBUTES; i += 4) {
    Log_info("-- coord (%f, %f), texture (%f, %f)",
             sprite->points[i], sprite->points[i+1],
             sprite->points[i+2], sprite->points[i+3]);
  }
  Log_info("------\n");
}

static float pixel_to_gl_coordX(float pixelX) {
  return (2.0f * (pixelX / Engine_get_width())) - 1;
}

static float pixel_to_gl_coordY(float pixelY) {
  return (2.0f * (pixelY / Engine_get_height())) - 1;
}

static Point point_to_gl(Point point)
{
  point.x = pixel_to_gl_coordX(point.x);
  point.y = pixel_to_gl_coordY(point.y);
  return point;
}

static float pixel_to_tex_coordX(Sprite *sprite, float pixelX) {
  return pixelX / sprite->texture_width;
}

static float pixel_to_tex_coordY(Sprite *sprite, float pixelY) {
  // Assume our image loader loads pixel rows top to bottom,
  // so we correct for that here but subtracting result from 1.
  return 1 - pixelY / sprite->texture_height;
}

static Point rotate_point(float angle, float x, float y,
                          float center_x, float center_y)
{
  Point p;
  float s = sinf(angle);
  float c = cosf(angle);

  // translate point back to origin:
  p.x = x - center_x;
  p.y = y - center_y;

  // rotate point
  float xnew = p.x * c - p.y * s;
  float ynew = p.x * s + p.y * c;

  // translate point back:
  p.x = xnew + center_x;
  p.y = ynew + center_y;
  return p;
}

static void update_points(Sprite *sprite)
{
  float leftX  = sprite->x - (sprite->width / 2);
  float rightX = sprite->x + (sprite->width / 2);
  float upperY = sprite->y + (sprite->height / 2);
  float lowerY = sprite->y - (sprite->height / 2);

  Point lowerLeft  = point_to_gl(rotate_point(sprite->rotation, leftX,
                                              lowerY, sprite->x, sprite->y));
  Point upperLeft  = point_to_gl(rotate_point(sprite->rotation, leftX,
                                              upperY, sprite->x, sprite->y));
  Point lowerRight = point_to_gl(rotate_point(sprite->rotation, rightX,
                                              lowerY, sprite->x, sprite->y));
  Point upperRight = point_to_gl(rotate_point(sprite->rotation, rightX,
                                              upperY, sprite->x, sprite->y));

  // Lower left.
  sprite->points[0] = lowerLeft.x;
  sprite->points[1] = lowerLeft.y;
  // Upper left.
  sprite->points[4] = upperLeft.x;
  sprite->points[5] = upperLeft.y;
  // Lower right.
  sprite->points[8] = lowerRight.x;
  sprite->points[9] = lowerRight.y;

  // Upper right.
  sprite->points[12] = upperRight.x;
  sprite->points[13] = upperRight.y;
  // Upper left.
  sprite->points[16] = upperLeft.x;
  sprite->points[17] = upperLeft.y;
  // Lower right.
  sprite->points[20] = lowerRight.x;
  sprite->points[21] = lowerRight.y;
}

static void update_tex_coords(Sprite *sprite)
{
  float *current_frame_coords = sprite->frame_coords +
    (sprite->current_frame * SPRITE_COORDS_PER_FRAME);

  float tex_leftX  = *current_frame_coords;
  float tex_lowerY = *(current_frame_coords + 1);
  float tex_rightX = *(current_frame_coords + 2);
  float tex_upperY = *(current_frame_coords + 3);

  // Lower left.
  sprite->points[2] = tex_leftX;
  sprite->points[3] = tex_lowerY;
  // Upper left.
  sprite->points[6] = tex_leftX;
  sprite->points[7] = tex_upperY;
  // Lower right.
  sprite->points[10] = tex_rightX;
  sprite->points[11] = tex_lowerY;
  // Upper right.
  sprite->points[14] = tex_rightX;
  sprite->points[15] = tex_upperY;
  // Upper left.
  sprite->points[18] = tex_leftX;
  sprite->points[19] = tex_upperY;
  // Lower right.
  sprite->points[22] = tex_rightX;
  sprite->points[23] = tex_lowerY;
}

static void set_no_frames(Sprite *sprite)
{
  int default_texture_coords[] = {
    0, 0, sprite->texture_width, sprite->texture_height
  };
  Sprite_create_frames(sprite, 1, default_texture_coords);
}

Sprite* Sprite_new(const char *filename, int width, int height)
{
  Sprite *sprite = malloc(sizeof(Sprite));
  sprite->width = width;
  sprite->height = height;
  sprite->x = 0;
  sprite->y = 0;
  sprite->rotation = 0;
  sprite->frame_count = 1;
  sprite->frame_coords = NULL;
  sprite->current_frame = 0;
  sprite->animation_start = -1.0f;

  if (!Engine_register_sprite(sprite, filename)) {
    Log("Unable to register new sprite");
    return 0;
  }

  sprite->texture_width = Engine_get_texture_width(filename);
  sprite->texture_height = Engine_get_texture_height(filename);

  // If no height was specified, use the image's original ratio.
  if (height == 0) {
    height = width * (float)sprite->texture_height/(float)sprite->texture_width;
  }

  set_no_frames(sprite);
  update_points(sprite);
  return sprite;
}

void Sprite_set_position(Sprite *sprite, float x, float y)
{
  sprite->x = x;
  sprite->y = y;
  update_points(sprite);
  Engine_update_sprite(sprite);
}

void Sprite_create_frames(Sprite *sprite, int frame_count, int *coords)
{
  sprite->frame_count = frame_count;

  // Allocate and store texture coordinates for frames in sprite struct.
  if (sprite->frame_coords) {
    free(sprite->frame_coords);
  }
  int coordinate_count = SPRITE_COORDS_PER_FRAME * frame_count;
  sprite->frame_coords = malloc(coordinate_count * sizeof(float));

  for (int i = 0; i < coordinate_count; i += 2) {
    sprite->frame_coords[i] = pixel_to_tex_coordX(sprite, coords[i]);
    sprite->frame_coords[i + 1] = pixel_to_tex_coordY(sprite, coords[i + 1]);
  }
  Sprite_set_frame(sprite, 0);
}

void Sprite_set_frame(Sprite *sprite, int frame_number)
{
  if (frame_number >= sprite->frame_count) {
    Log("Sprite ERROR: invalid frame number");
    return;
  }
  sprite->current_frame = frame_number;
  update_tex_coords(sprite);
  Engine_update_sprite(sprite);
}

void Sprite_animate(Sprite *sprite, float duration)
{
  sprite->animation_start = Engine_get_elapsed_time();
  sprite->animation_duration = duration / sprite->frame_count;
}

void Sprite_tick(Sprite *sprite, double elapsed)
{
  Sprite s = *sprite;
  double frame = (elapsed - s.animation_start) / s.animation_duration;
  Sprite_set_frame(sprite, (int)frame % s.frame_count);
}
