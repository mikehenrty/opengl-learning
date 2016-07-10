#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "sprite.h"

typedef struct Background {
  Sprite *sprite;
  float pixels_per_second;
  double last_update;
} Background;

Background* Background_new(const char *filename, int width, int height);

void Background_set_speed(Background *background, float pps);
void Background_set_position(Background *background, float y);
void Background_tick(Background *background, double elapsed);

#endif
