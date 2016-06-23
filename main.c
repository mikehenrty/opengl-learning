#include <math.h>
#include <stdlib.h>
#include <time.h>

#define GAME_NAME "Jumpy"
#define GAME_WIDTH  800
#define GAME_HEIGHT 600
// TODO: remove this, it's just for testing purposes.
#define TEMP_NUM_SPRITES 1000

#include "gl.h"
#include "logger.h"
#include "engine.h"
#include "sprite.h"

typedef struct sprite_attribs {
  int start_x;
  int start_y;
  int animation;
} sprite_attribs;

static Sprite *sprites[MAX_SPRITES];
static sprite_attribs attribs[MAX_SPRITES * 3];
static int sprite_count = 0;

static void key_callback(int key)
{
  // TODO: do something with the keys once we figure out function pointers.
}

int get_random_number(int min, int max)
{
  static int seed = 0;
  if (seed == 0) {
    seed = time(NULL);
    srand((unsigned)seed);
  }
  return rand() % (max - min) + min;
}

Sprite *create_random_sprite()
{
  int dimension = get_random_number(50, 150);
  char * filename;
  if (sprite_count % 2 == 1) {
    filename = "data/hello.tga";
  } else {
    filename = "data/hello2.tga";
  }
  Sprite *sprite = Sprite_new(filename, dimension, dimension);

  int start_x = get_random_number(-200, GAME_WIDTH + 200);
  int start_y = get_random_number(-200, GAME_HEIGHT + 200);
  attribs[sprite_count].start_x = start_x;
  attribs[sprite_count].start_y = start_y;
  Sprite_set_position(sprite, start_x, start_y);
  attribs[sprite_count].animation = get_random_number(0, 2);

  ++sprite_count;
  return sprite;
}


int init_world()
{
  for (int i = 0; i < TEMP_NUM_SPRITES; i++) {
    sprites[i] = create_random_sprite();
    if (!sprites[i]) {
      Log("Sprite creation failed");
      return 0;
    }
  }
  return 1;
}

void update_world() {
  double elapsed = Engine_get_elapsed_time();
  float new_x, new_y;

  for (int i = 0; i < sprite_count; i++) {
    if (attribs[i].animation == 1) {
      new_x = attribs[i].start_x + sinf((float)elapsed * 4) * 200.0f;
      new_y = attribs[i].start_y + cosf((float)elapsed * 2) * 200.0f;
    } else {
      new_x = attribs[i].start_x + cosf((float)elapsed * 4) * 200.0f;
      new_y = attribs[i].start_y + sinf((float)elapsed * 2) * 200.0f;
    }
    Sprite_set_position(sprites[i], new_x, new_y);
  }
}

int init_engine()
{
  if (!Engine_init(GAME_NAME, GAME_WIDTH, GAME_HEIGHT)) {
    return 0;
  }
  Engine_print_hardware_info();
  Engine_log_fps();
  Engine_set_key_callback(key_callback);
  return 1;
}

void start_main_loop()
{
  while (Engine_is_running()) {
    update_world();
    Engine_draw_everything();
    Engine_print_gl_error("Rendering");
  }

  // Now that the loop has excited, print any logs.
  Engine_print_program_log();
}

int main(int argc, char** argv)
{
  if (!init_engine()) {
    Log("Unable to init engine");
    return -1;
  }

  if(!init_world()) {
    Log("Unable to create world, exiting");
    return -1;
  }

  start_main_loop();
  return 0;
}
