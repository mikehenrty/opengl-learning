#include <stdlib.h>

#include "utils.h"
#include "logger.h"
#include "sprite.h"
#include "engine.h"
#include "clouds.h"

static double last_update = 0;

typedef struct Cloud {
  Sprite *sprite;
  unsigned velocity;
} Cloud;

static unsigned cloud_count = 0;
static Cloud *clouds[MAX_CLOUDS];

unsigned get_random_y()
{
  return Utils_random_int(100, Engine_get_height());
}

Cloud* create_cloud() {
  Cloud *cloud = malloc(sizeof(Cloud));
  //cloud->sprite = Sprite_new("data/cloud.png", 200, 0);
  cloud->sprite = Sprite_new("data/cloud.png", 200, 200);
  Sprite_set_position(cloud->sprite,
                      Utils_random_int(0, Engine_get_width()),
                      get_random_y());
  cloud->velocity = Utils_random_int(100, 150);
  return cloud;
}

int Clouds_create(unsigned count)
{
  if (cloud_count == 0 && count > 0) {
    Engine_register_tick_callback(NULL, Clouds_tick);
  }

  for (unsigned i = 0; i < count; i++) {
    Cloud *cloud = create_cloud();
    if (!cloud) {
      Log("Unable to create cloud");
      return 0;
    }

    clouds[cloud_count] = cloud;
    ++cloud_count;
  }

  return 1;
}

void Clouds_tick(void * unused, double elapsed)
{
  for (unsigned i = 0; i < cloud_count; i++) {
    Cloud *c = clouds[i];
    int delta = (elapsed - last_update) * c->velocity;
    int new_x = c->sprite->x - delta;
    int new_y = c->sprite->y;
    if (new_x < 0 - (int)(c->sprite->width) / 2) {
      new_x = Engine_get_width() + c->sprite->width / 2;
      new_y = get_random_y();
    }
    Sprite_set_position(c->sprite, new_x, new_y);
  }
  last_update = elapsed;
}
