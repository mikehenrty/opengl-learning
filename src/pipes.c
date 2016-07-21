#include <stdlib.h>

#include "engine.h"
#include "pipe.h"
#include "pipes.h"

static float pipe_interval;
static unsigned current_pipe = 0;
static double time_since_release = 0;

Pipes Pipes_create(float interval, float velocity)
{
  Pipes pipes = (Pipe *)malloc(sizeof(Pipe) * PIPE_COUNT);
  pipe_interval = interval;

  pipes[0] = *Pipe_new(velocity);
  pipes[1] = *Pipe_new(velocity);
  pipes[2] = *Pipe_new(velocity);
  pipes[3] = *Pipe_new(velocity);

  return pipes;
}

static void pipes_tick(void *p, double elapsed, double since)
{
  Pipes pipes = (Pipes)p;
  time_since_release += since;
  if (time_since_release > pipe_interval) {
    time_since_release -= pipe_interval;
    Pipe_release(&pipes[current_pipe]);
    current_pipe = (current_pipe + 1) % PIPE_COUNT;
  }

  for (int i = 0; i < PIPE_COUNT; i++) {
    Pipe_tick((void *)&pipes[i], elapsed, since);
  }
}

void Pipes_go(Pipes pipes)
{
  Engine_register_tick_callback((void *)pipes, pipes_tick);
}
