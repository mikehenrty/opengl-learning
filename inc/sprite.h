#ifndef SPRITE_H
#define SPRITE_H

#define SPRITE_NUM_INDICES 6
#define SPRITE_NUM_ATT_PER_INDEX 4
#define SPRITE_NUM_ATTRIBUTES SPRITE_NUM_ATT_PER_INDEX * SPRITE_NUM_INDICES
#define SPRITE_SIZE SPRITE_NUM_ATTRIBUTES * sizeof(GLfloat)

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
