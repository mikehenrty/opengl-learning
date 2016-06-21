#include <math.h>

#include "gl.h"
#include "logger.h"
#include "engine.h"
#include "sprite.h"

static int count = 0;
static Sprite *sprite;

static void key_callback(int key)
{
  // TODO: do something with the keys once we figure out function pointers.
}


void init_world()
{
  sprite = Sprite_new(100, 100);
}

void render_world() {
  Sprite_set_position(sprite, sinf((float)count * 0.1) * 50.0f,
                              sinf((float)count * 0.1) * 50.0f);
  Sprite_render(sprite);
}

void start_main_loop()
{
  while (Engine_is_running()) {
    render_world();
    Engine_draw_everything();
    Engine_print_gl_error("Rendering");
    ++count;
  }
}


int main(int argc, char** argv)
{
  int result = Engine_init(800, 600);
  if (!result) {
    return -1;
  }

  Log_info("Running app");
  Engine_print_hardware_info();
  Engine_log_fps();
  Engine_set_key_callback(key_callback);

  init_world();
  start_main_loop();

  Engine_print_program_log();
  return 0;
}
