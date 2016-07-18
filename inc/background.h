#ifndef BACKGROUND_H
#define BACKGROUND_H

#define MAX_BACKGROUNDS 5

#include "sprite.h"

typedef struct Background {
  Sprite *sprite;
  float pixels_per_second;
} Background;

int Background_create_full(const char *filename, float pps);
int Background_create_parallax(const char *filename, float pps,
                               int height, float y);

void Background_set_speed(Background *background, float pps);
void Background_set_position(Background *background, float y);

#endif
