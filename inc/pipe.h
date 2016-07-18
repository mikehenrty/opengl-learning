#ifndef PIPE_H
#define PIPE_H

#define PIPE_WIDTH 190
#define PIPE_VELOCITY 700

#include "sprite.h"

typedef struct Pipe {
  Sprite *sprite;
  float velocity_x;
} Pipe;

Pipe* Pipe_new();
void Pipe_release(Pipe *pipe);

#endif
