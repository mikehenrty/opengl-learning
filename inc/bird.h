#ifndef BIRD_H
#define BIRD_H

#define GRAVITY -2100 // pixels per second.
#define FLAP_POWER 140
#define MAX_VELOCITY 1000
#define ROTATION_FACTOR 35 // degrees of rotation based on velocity
#define BIRD_RATIO_W_H 0.677f

typedef struct Bird {
  Sprite *sprite;
  float   velocity_y;
} Bird;

Bird* Bird_new(int width);
void  Bird_set_position(Bird *bird, float x, float y);
void  Bird_set_rotation(Bird *bird, float rotation);
void  Bird_flap(Bird *bird);

#endif
