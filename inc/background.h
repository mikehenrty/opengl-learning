#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "sprite.h"

typedef struct Background {
  Sprite *sprite;
} Background;

Background* Background_new(const char *filename);

void Background_set_speed(float pps);
void Background_tick(Background *background, double elapsed);

#endif
