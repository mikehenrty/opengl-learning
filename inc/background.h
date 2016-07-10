#ifndef BACKGROUND_H
#define BACKGROUND_H

typedef struct Background {
  Sprite *sprite;
} Background;

Background* Background_new(const char *filename);

#endif
