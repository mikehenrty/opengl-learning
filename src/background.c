#include <stdlib.h>

#include "engine.h"
#include "background.h"

Background* Background_new(const char *filename)
{
  Background *background = malloc(sizeof(Background));
  background->sprite = Sprite_new(filename, Engine_get_width(), Engine_get_height());
  Sprite_set_position(background->sprite,
                      Engine_get_width() / 2,
                      Engine_get_height() / 2);
  return background;
}
