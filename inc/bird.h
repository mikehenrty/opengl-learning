#ifndef BIRD_H
#define BIRD_H

#define BIRD_RATIO_W_H 0.677f

typedef struct Bird {
  Sprite *sprite;
} Bird;

Bird* Bird_new(int width);
void Bird_set_position(Bird *bird, float x, float y);
void Bird_set_rotation(Bird *bird, float rotation);

#endif
