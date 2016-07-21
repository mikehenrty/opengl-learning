#ifndef PIPE_H
#define PIPE_H

#define PIPE_WIDTH 190

#include "sprite.h"

typedef struct Pipe {
  Sprite   *sprite;
  float     velocity_x;
  unsigned  active;
} Pipe;

Pipe* Pipe_new(float velocity);
void  Pipe_tick(void *p, double elapsed, double since);
void  Pipe_release(Pipe *pipe);

#endif
