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
  Log_info("------\n");
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
  // Assume our image loader loads pixel rows top to bottom,
  // so we correct for that here but subtracting result from 1.
  return 1 - pixelY / sprite->texture_height;
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
  float tex_bottomY    = *(current_frame_coords + 1);
  float tex_rightX  = *(current_frame_coords + 2);
  float tex_topY = *(current_frame_coords + 3);

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
