#include <stdlib.h>

#include "sprite.h"
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

Bird* Bird_new(int width)
{
  Bird* bird = malloc(sizeof(Bird));
  bird->sprite = Sprite_new("data/bird.png",
                            width,
                            (int)(width * BIRD_RATIO_W_H));
  Sprite_create_frames(bird->sprite, 8, tex_coords);
  Sprite_animate(bird->sprite, 0.4);
  return bird;
}

void Bird_set_position(Bird *bird, float x, float y)
{
  Sprite_set_position(bird->sprite, x, y);
}
