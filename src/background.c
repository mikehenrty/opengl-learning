#include <stdlib.h>

#include "logger.h"
#include "engine.h"
#include "background.h"

static unsigned background_count = 0;
static Background *backgrounds[MAX_BACKGROUNDS];

static Background* Background_new(const char *filename, int width, int height)
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

static void Background_tick(void *b, double elapsed) {
  Background *background = (Background *)b;

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

static int create_background(const char *filename, float pps,
                             int width, int height)
{
  if (background_count == MAX_BACKGROUNDS) {
    Log("Unable to create new background %s, max already created", filename);
    return 0;
  }

  int background_index = background_count++;
  Background *b = Background_new(filename, width, height);
  backgrounds[background_index] = b;
  Background_set_speed(b, pps);
  Engine_register_tick_callback(b, Background_tick);
  return background_index;
}

int Background_create_full(const char *filename, float pps)
{
  return create_background(filename, pps, 0, 0);
}

int Background_create_parallax(const char *filename, float pps,
                               int height, float y)
{
  int background_index = create_background(filename, pps, 0, height);
  if (!background_index) {
    Log("Unable to create background for parralax");
    return 0;
  }
  Background_set_position(backgrounds[background_index], y);
  return background_index;
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

