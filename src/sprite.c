#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "engine.h"
#include "sprite.h"

static void print_points(Sprite *sprite)
{
  for (int i = 0; i < SPRITE_NUM_ATTRIBUTES; i += 4) {
    Log_info("-- coord (%f, %f), texture (%f, %f)",
             sprite->points[i], sprite->points[i+1],
             sprite->points[i+2], sprite->points[i+3]);
  }
}

static float pixel_to_gl_coordX(float pixelX) {
  return (2.0f * (pixelX / Engine_get_width())) - 1;
}

static float pixel_to_gl_coordY(float pixelY) {
  return (2.0f * (pixelY / Engine_get_height())) - 1;
}

static float pixel_to_tex_coordX(Sprite *sprite, float pixelX) {
  return pixelX / sprite->texture_width;
}

static float pixel_to_tex_coordY(Sprite *sprite, float pixelY) {
  return pixelY / sprite->texture_height;
}

static void update_points(Sprite *sprite)
{
  float leftX   = pixel_to_gl_coordX(sprite->x - (sprite->width / 2));
  float rightX  = pixel_to_gl_coordX(sprite->x + (sprite->width / 2));
  float topY    = pixel_to_gl_coordY(sprite->y + (sprite->height / 2));
  float bottomY = pixel_to_gl_coordY(sprite->y - (sprite->height / 2));

  // Lower left.
  sprite->points[0] = leftX;
  sprite->points[1] = bottomY;
  // Upper left.
  sprite->points[4] = leftX;
  sprite->points[5] = topY;
  // Lower right.
  sprite->points[8] = rightX;
  sprite->points[9] = bottomY;
  // Upper right.
  sprite->points[12] = rightX;
  sprite->points[13] = topY;
  // Upper left.
  sprite->points[16] = leftX;
  sprite->points[17] = topY;
  // Lower right.
  sprite->points[20] = rightX;
  sprite->points[21] = bottomY;
}

static void update_tex_coords(Sprite *sprite)
{
  float *current_frame_coords = sprite->frame_coords +
    (sprite->current_frame * SPRITE_COORDS_PER_FRAME);

  float tex_leftX   = *current_frame_coords;
  float tex_topY    = *(current_frame_coords + 1);
  float tex_rightX  = *(current_frame_coords + 2);
  float tex_bottomY = *(current_frame_coords + 3);

  // Lower left.
  sprite->points[2] = tex_leftX;
  sprite->points[3] = tex_bottomY;
  // Upper left.
  sprite->points[6] = tex_leftX;
  sprite->points[7] = tex_topY;
  // Lower right.
  sprite->points[10] = tex_rightX;
  sprite->points[11] = tex_bottomY;
  // Upper right.
  sprite->points[14] = tex_rightX;
  sprite->points[15] = tex_topY;
  // Upper left.
  sprite->points[18] = tex_leftX;
  sprite->points[19] = tex_topY;
  // Lower right.
  sprite->points[22] = tex_rightX;
  sprite->points[23] = tex_bottomY;
}

Sprite* Sprite_new(const char *filename, int width, int height)
{
  Sprite *sprite = malloc(sizeof(Sprite));
  sprite->width = width;
  sprite->height = height;
  sprite->x = 0;
  sprite->y = 0;
  sprite->frame_count = 1;
  sprite->current_frame = 0;
  sprite->animation_start = -1.0f;
  if (!Engine_register_sprite(sprite, filename)) {
    Log("Unable to register new sprite");
    return 0;
  }
  sprite->texture_width = Engine_get_texture_width(filename);
  sprite->texture_height = Engine_get_texture_height(filename);
  int default_texture_coords[] = {
    0, 0, sprite->texture_width, sprite->texture_height
  };
  Sprite_create_frames(sprite, 1, 0, default_texture_coords);
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

void Sprite_create_frames(Sprite *sprite, int frame_count,
                          float duration, int *coords)
{
  sprite->frame_count = frame_count;
  sprite->animation_duration = duration / frame_count;
  sprite->current_frame = 0;

  // Allocate and store texture coordinates for frames in sprite struct.
  int coordinate_count = SPRITE_COORDS_PER_FRAME * frame_count;
  sprite->frame_coords = malloc(coordinate_count * sizeof(float));
  for (int i = 0; i < coordinate_count; i = i + 2) {
    sprite->frame_coords[i] = pixel_to_tex_coordX(sprite, coords[i]);
    sprite->frame_coords[i + 1] = pixel_to_tex_coordY(sprite, coords[i + 1]);
  }
  update_tex_coords(sprite);
}

void Sprite_animate(Sprite *sprite)
{
  sprite->animation_start = Engine_get_elapsed_time();
}

void Sprite_tick(Sprite *sprite, double elapsed)
{
  Sprite s = *sprite;
  double frame = (elapsed - s.animation_start) / s.animation_duration;
  sprite->current_frame = (int)frame % s.frame_count;
  update_tex_coords(sprite);
  Engine_update_sprite(sprite);
}
