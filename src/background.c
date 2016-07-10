#include <stdlib.h>

#include "logger.h"
#include "engine.h"
#include "background.h"

static float pixels_per_second = 0;
static double last_update = 0;

Background* Background_new(const char *filename)
{
  Background *background = malloc(sizeof(Background));
  background->sprite = Sprite_new(filename,
                                  Engine_get_width() * 2,
                                  Engine_get_height());

  Sprite_set_position(background->sprite,
                      Engine_get_width(),
                      Engine_get_height() / 2);

  // Use special texture coordinates to set up a repeating background pattern.
  int special_texture_coords[] = {
    0, 0, background->sprite->texture_width * 2, background->sprite->texture_height
  };
  Sprite_create_frames(background->sprite, 1, special_texture_coords);

  return background;
}

void Background_set_speed(float pps)
{
  pixels_per_second = pps;
}

void Background_tick(Background *background, double elapsed) {
  double elapsed_since_update = elapsed - last_update;
  float new_x = background->sprite->x - elapsed_since_update * pixels_per_second;
  if (new_x < 0) {
    new_x += Engine_get_width();
  }
  Sprite_set_position(background->sprite, new_x, background->sprite->y);
  last_update = elapsed;
}
