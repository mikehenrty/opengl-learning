#include <math.h>

#define GAME_WIDTH  800
#define GAME_HEIGHT 600

#include "gl.h"
#include "logger.h"
#include "engine.h"
#include "sprite.h"

static Sprite *sprite;

static void key_callback(int key)
{
  // TODO: do something with the keys once we figure out function pointers.
}


int init_world()
{
  sprite = Sprite_new(100, 100);
  if (!sprite) {
    Log("Sprite creation failed");
    return 0;
  }
  return 1;
}

void update_world() {
  double elapsed = Engine_get_elapsed_time();
  Sprite_set_position(sprite, 400 + sinf((float)elapsed * 4) * 200.0f,
                              300 + cosf((float)elapsed * 2) * 200.0f);
}

void start_main_loop()
{
  while (Engine_is_running()) {
    update_world();
    Engine_draw_everything();
    Engine_print_gl_error("Rendering");
  }
}


int main(int argc, char** argv)
{
  int result = Engine_init(GAME_WIDTH, GAME_HEIGHT);
  if (!result) {
    return -1;
  }

  Log_info("Running app");
  Engine_print_hardware_info();
  Engine_log_fps();
  Engine_set_key_callback(key_callback);

  if(!init_world()) {
    Log("Unable to create world, exiting");
    return -1;
  }

  start_main_loop();
  Engine_print_program_log();
  return 0;
}
