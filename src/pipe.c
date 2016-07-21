#include <stdlib.h>

#include "utils.h"
#include "engine.h"
#include "pipe.h"

static float random_y()
{
  return Utils_random_int(30, 200);
}

static void Pipe_reset_position(Pipe *pipe)
{
  Sprite_set_position(pipe->sprite,
                      Engine_get_width() + PIPE_WIDTH / 2, random_y());
}

void Pipe_release(Pipe *pipe)
{
  Pipe_reset_position(pipe);
  pipe->active = 1;
}

void Pipe_tick(void *p, double elapsed, double since)
{
  Pipe *pipe = (Pipe *)p;
  if (pipe->active) {
    float new_x = pipe->sprite->x + (pipe->velocity_x * since);
    Sprite_set_position(pipe->sprite,
                        pipe->sprite->x + pipe->velocity_x * since,
                        pipe->sprite->y);
    if (new_x + PIPE_WIDTH / 2 < 0) {
      pipe->active = 0;
    }
  }
}

Pipe* Pipe_new(float velocity)
{
  Pipe *pipe = malloc(sizeof(Pipe));
  pipe->sprite = Sprite_new("data/pipe.png", PIPE_WIDTH, 0);
  pipe->velocity_x = -velocity;
  pipe->active = 0;
  Pipe_reset_position(pipe);

  return pipe;
}
