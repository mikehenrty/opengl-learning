#include <stdlib.h>

#include "utils.h"
#include "engine.h"
#include "pipe.h"

static float random_y()
{
  return Utils_random_int(30, 200);
}

void Pipe_release(Pipe *pipe)
{
  Sprite_set_position(pipe->sprite,
                      Engine_get_width() + PIPE_WIDTH / 2, random_y());
}

static void tick(void *p, double elapsed, double since)
{
  Pipe *pipe = (Pipe *)p;
  float new_x = pipe->sprite->x + (pipe->velocity_x * since);
  if (new_x + PIPE_WIDTH / 2 < 0) {
    Pipe_release(pipe);
  }
  Sprite_set_position(pipe->sprite, pipe->sprite->x + pipe->velocity_x * since,
                                    pipe->sprite->y);
}

Pipe* Pipe_new()
{
  Pipe *pipe = malloc(sizeof(Pipe));
  pipe->sprite = Sprite_new("data/pipe.png", PIPE_WIDTH, 0);
  pipe->sprite->y = random_y();
  pipe->velocity_x = -PIPE_VELOCITY;

  Engine_register_tick_callback((void *)pipe, &tick);
  Pipe_release(pipe);
  return pipe;
}
