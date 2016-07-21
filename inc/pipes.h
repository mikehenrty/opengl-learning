#ifndef PIPES_H
#define PIPES_H

#include "pipe.h"

#define PIPE_COUNT 4

typedef Pipe *Pipes;

Pipes Pipes_create(float interval, float velocity);
void Pipes_go(Pipes pipes);

#endif
