#ifndef SPRITE_H
#define SPRITE_H

#define SPRITE_NUM_INDICES 6
#define SPRITE_NUM_ATT_PER_INDEX 4
#define SPRITE_NUM_ATTRIBUTES SPRITE_NUM_ATT_PER_INDEX * SPRITE_NUM_INDICES
#define SPRITE_SIZE SPRITE_NUM_ATTRIBUTES * sizeof(GLfloat)

// Upper left x and y, plus lower right x and y. texture coordinates.
#define SPRITE_COORDS_PER_FRAME 4

#include "gl.h"

typedef struct Sprite {
  unsigned int width;
  unsigned int height;
  float x;
  float y;
  float rotation;
  GLfloat *points;

  unsigned int frame_count;
  unsigned int current_frame;
  float *frame_coords;
  double animation_start;
  float animation_duration;
  unsigned texture_width;
  unsigned texture_height;
} Sprite;

Sprite* Sprite_new(const char *filename, int width, int height);
void Sprite_render(Sprite *sprite);
void Sprite_set_position(Sprite *sprite, float x, float y);
void Sprite_create_frames(Sprite *sprite, int frame_count, int *coordinates);
void Sprite_set_frame(Sprite *sprite, int frame_number);
void Sprite_animate(Sprite *sprite, float duration);

#endif
