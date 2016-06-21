#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <GLFW/glfw3.h>
#include "logger.h"
#include "engine.h"
#include "sprite.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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
  double elapsed = Engine_get_elapsed_time();
  Sprite_set_position(sprite, 400 + sinf((float)elapsed * 4) * 200.0f,
                              300 + cosf((float)elapsed * 2) * 200.0f);
  Sprite_render(sprite);
}

void start_main_loop()
{
    render_world();
    Engine_draw_everything();
    Engine_print_gl_error("Smendering");
}


int main(int argc, char** argv)
{
  int result = Engine_init(800, 600);
  if (!result) {
    return -1;
  }
  Engine_print_gl_error("after init");

  Log_info("Running app");
  //Engine_print_hardware_info();
  Engine_print_gl_error("hardware");
  Engine_log_fps();
  Engine_print_gl_error("fps");
  Engine_set_key_callback(key_callback);
  Engine_print_gl_error("keycallback");

  Engine_print_gl_error("basic setup");

  init_world();
  Engine_print_gl_error("after world");
  emscripten_set_main_loop(start_main_loop, 0, 1);
  start_main_loop();

  Engine_print_program_log();
  return 0;
}
