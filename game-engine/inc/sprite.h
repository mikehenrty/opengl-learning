#ifndef SPRITE_H
#define SPRITE_H

#include "gl.h"

// Values used by the shader to determine texture coords.
#define LOWER_LEFT  -1.0f
#define UPPER_LEFT  -0.5f
#define LOWER_RIGHT  0.5f
#define UPPER_RIGHT  1.0f

typedef struct Sprite {
  unsigned int width;
  unsigned int height;
  float x;
  float y;
  GLfloat points[18];
  GLuint vbo;
} Sprite;

Sprite* Sprite_new();
void Sprite_render(Sprite *sprite);
void Sprite_set_position(Sprite *sprite, float x, float y);

#endif
