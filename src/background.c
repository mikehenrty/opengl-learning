#include <stdlib.h>

#include "logger.h"
#include "engine.h"
#include "background.h"

Background* Background_new(const char *filename, int width, int height)
{
  if (width == 0) {
    width = Engine_get_width() * 2;
  }

  if (height == 0) {
    height = Engine_get_height();
  }

  Background *background = malloc(sizeof(Background));
  background->sprite = Sprite_new(filename, width, height);
  Sprite_set_position(background->sprite,
                      Engine_get_width(),
                      Engine_get_height() / 2);

  // Use special texture coordinates to set up a repeating background pattern.
  int special_texture_coords[] = {
    0, 0,
    background->sprite->texture_width * 2,
    background->sprite->texture_height
  };
  Sprite_create_frames(background->sprite, 1, special_texture_coords);

  return background;
}

void Background_set_speed(Background *background, float pps)
{
  background->pixels_per_second = pps;
}

void Background_set_position(Background *background, float y)
{
  int special_texture_coords[] = {
    0, 0,
    background->sprite->texture_width * 2,
    background->sprite->texture_height
  };
  Sprite_create_frames(background->sprite, 1, special_texture_coords);
  Sprite *s = background->sprite;
  Sprite_set_position(s, s->x, y);
}

void Background_tick(Background *background, double elapsed) {
  double elapsed_since_update = elapsed - background->last_update;
  float new_x = background->sprite->x -
                elapsed_since_update *
                background->pixels_per_second;
  if (new_x < 0) {
    new_x += Engine_get_width();
  }
  Sprite_set_position(background->sprite, new_x, background->sprite->y);
  background->last_update = elapsed;
}
