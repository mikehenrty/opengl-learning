#include <stdlib.h>
#include <math.h>

#include "engine.h"
#include "bird.h"

static int tex_coords[] = {
  0,   225, 110,  298,
  109, 225, 219,  298,
  0,   153, 110,  226,
  109, 153, 219,  226,
  0,   80,  110,  154,
  109, 80,  219,  154,
  0,   0,   110,  72,
  109, 0,   219,  72
};

static void tick(void *b, double elapsed)
{
  Bird *bird = (Bird *)b;
  static double last_update = 0.0;
  double elapsed_since = elapsed - last_update;
  last_update = elapsed;
  bird->velocity_y += GRAVITY * elapsed_since;
  bird->sprite->y += bird->velocity_y * elapsed_since;

  if (bird->sprite->y < bird->sprite->height / 2) {
    bird->sprite->y = bird->sprite->height / 2;
    bird->velocity_y = 0;
  }
  Sprite_set_position(bird->sprite, bird->sprite->x, bird->sprite->y);
}

Bird* Bird_new(int width)
{
  Bird* bird = malloc(sizeof(Bird));
  bird->velocity_y = 0;
  bird->sprite = Sprite_new("data/bird.png",
                            width,
                            (int)(width * BIRD_RATIO_W_H));
  Sprite_create_frames(bird->sprite, 8, tex_coords);
  Sprite_animate(bird->sprite, 0.4);
  Engine_register_tick_callback((void *)bird, &tick);
  return bird;
}

void Bird_set_position(Bird *bird, float x, float y)
{
  Sprite_set_position(bird->sprite, x, y);
}

void Bird_set_rotation(Bird *bird, float rotation)
{
  bird->sprite->rotation = rotation;
}

void Bird_flap(Bird *bird)
{
  bird->velocity_y += FLAP_POWER;
  if (bird->velocity_y > MAX_VELOCITY) {
    bird->velocity_y = MAX_VELOCITY;
  }
}
