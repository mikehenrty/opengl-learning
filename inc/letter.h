#ifndef LETTER_H
#define LETTER_H

#include "sprite.h"

typedef struct Letter {
  Sprite *sprite;
} Letter;

Letter* Letter_new(int width, int height);
void Letter_display(Letter *letter, char character);
void Letter_set_position(Letter *letter, float x, float y);

#endif
