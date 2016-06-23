#ifndef SPRITE_H
#define SPRITE_H

#define SPRITE_NUM_INDICES 6
#define SPRITE_NUM_ATT_PER_INDEX 3
#define SPRITE_NUM_ATTRIBUTES SPRITE_NUM_ATT_PER_INDEX * SPRITE_NUM_INDICES
#define SPRITE_SIZE SPRITE_NUM_ATTRIBUTES * sizeof(GLfloat)

// Values used by the shader to determine texture coords.
#define LOWER_LEFT  -1.0f
#define UPPER_LEFT  -0.5f
#define LOWER_RIGHT  0.5f
#define UPPER_RIGHT  1.0f

#include "gl.h"

typedef struct Sprite {
  unsigned int width;
  unsigned int height;
  float x;
  float y;
  GLfloat *points;
} Sprite;

Sprite* Sprite_new(const char *filename, int width, int height);
void Sprite_render(Sprite *sprite);
void Sprite_set_position(Sprite *sprite, float x, float y);

#endif
