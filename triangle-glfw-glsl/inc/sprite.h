#ifndef SPRITE_H
#define SPRITE_H

#include "gl.h"

typedef struct Sprite {
  unsigned int width;
  unsigned int height;
  float x;
  float y;
  GLfloat points[12];
  GLuint vbo;
} Sprite;

Sprite* Sprite_new();
void Sprite_render(Sprite *sprite);
void Sprite_set_position(Sprite *sprite, float x, float y);

#endif
