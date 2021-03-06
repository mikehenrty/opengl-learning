#include <stdlib.h>
#include <math.h>

#define GAME_NAME   "Jumpy"
#define GAME_WIDTH  800
#define GAME_HEIGHT 600
#define BIRD_WIDTH  120
#define PIPE_VELOCITY 700
#define PIPE_INTERVAL 0.5

// TODO: remove this, it's just for testing purposes.
#define TEMP_NUM_SPRITES 40
#define NUM_CLOUDS       5

#include "gl.h"
#include "utils.h"
#include "logger.h"
#include "engine.h"
#include "bird.h"
#include "pipes.h"

static Pipes pipes;

typedef struct bird_attribs {
  int start_x;
  int start_y;
  int animation;
} bird_attribs;

static Bird *birds[MAX_SPRITES];
static bird_attribs attribs[MAX_SPRITES * 3];
static int bird_count = 0;

// Main character.
static Bird *main_bird;

static void key_callback(int key)
{
  if (key == GLFW_KEY_SPACE) {
    Bird_flap(main_bird);
  } else {
    Log_info("Unrecognized key press: %d", key);
  }
}

Bird *create_random_bird()
{
  int width = Utils_random_int(50, 150);
  Bird *bird = Bird_new(width);
  int start_x = Utils_random_int(0, GAME_WIDTH);
  int start_y = Utils_random_int(0, GAME_HEIGHT);
  attribs[bird_count].start_x = start_x;
  attribs[bird_count].start_y = start_y;
  Bird_set_position(bird, start_x, start_y);
  attribs[bird_count].animation = Utils_random_int(0, 2);

  ++bird_count;
  return bird;
}


int init_world()
{
  // Create the backdrop.
  Background_create_full("data/background-red.png", 40.0);
  Clouds_create(NUM_CLOUDS);

  // Create the characters.
  for (int i = 0; i < TEMP_NUM_SPRITES; i++) {
    birds[i] = create_random_bird();
    if (!birds[i]) {
      Log("Bird creation failed");
      return 0;
    }
  }

  // Trees, and birds and pipes.
  Background_create_parallax("data/trees.png", 1100.0, 250, 120.0);
  main_bird = Bird_new(BIRD_WIDTH);
  Bird_set_position(main_bird, GAME_WIDTH / 3, GAME_HEIGHT - GAME_HEIGHT / 3);
  pipes = Pipes_create(PIPE_INTERVAL, PIPE_VELOCITY);
  Pipes_go(pipes);

  return 1;
}

void update_world() {
  double elapsed = Engine_get_elapsed_time();
  float new_x, new_y;

  for (int i = 0; i < bird_count; i++) {
    if (Utils_roll(5.0)) {
      Bird_flap(birds[i]);
    }
  }
}

int init_engine()
{
  if (!Engine_init(GAME_NAME, GAME_WIDTH, GAME_HEIGHT)) {
    return 0;
  }
  Engine_print_hardware_info();
  Engine_set_key_callback(key_callback);
  return 1;
}

void start_main_loop()
{
  FPS_enable();
  while (Engine_is_running()) {
    update_world();
    Engine_tick();
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
