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

static void tick(void *b, double elapsed, double since)
{
  Bird *bird = (Bird *)b;
  bird->velocity_y += GRAVITY * since;
  bird->sprite->y += bird->velocity_y * since;

  float *y = &bird->sprite->y;
  unsigned half_height = bird->sprite->height / 2;
  if (*y < half_height) {
    *y = half_height;
    bird->velocity_y = -bird->velocity_y / 3;
  } else if (*y + half_height > Engine_get_height()) {
    *y = Engine_get_height() - half_height;
    // Dampen vertical velocity when hitting the ceiling.
    bird->velocity_y = bird->velocity_y / 2;
  }


  float rotation = sinf(90 * (bird->velocity_y / MAX_VELOCITY) *
                       (M_PI / 180)) * ROTATION_FACTOR;
  Bird_set_rotation(bird, rotation);
  Sprite_set_position(bird->sprite, bird->sprite->x, bird->sprite->y);
}

Bird* Bird_new(int width)
{
  Bird* bird = malloc(sizeof(Bird));
  bird->velocity_y = MAX_VELOCITY;
  bird->sprite = Sprite_new("data/bird.png", width, 0);
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
  bird->velocity_y += FLAP_POWER * bird->sprite->height;
  if (bird->velocity_y > MAX_VELOCITY) {
    bird->velocity_y = MAX_VELOCITY;
  }
}
