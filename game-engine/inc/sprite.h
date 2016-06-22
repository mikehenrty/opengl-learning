#ifndef SPRITE_H
#define SPRITE_H

#define SPRITE_NUM_POINTS 18
#define SPRITE_SIZE SPRITE_NUM_POINTS * sizeof(GLfloat)

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

Sprite* Sprite_new();
void Sprite_render(Sprite *sprite);
void Sprite_set_position(Sprite *sprite, float x, float y);

#endif
