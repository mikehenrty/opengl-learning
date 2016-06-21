#include <math.h>

#include "gl.h"
#include "logger.h"
#include "engine.h"

static int count = 0;
static GLuint vertex_buffer = 0;
static GLuint vertex_array = 0;

static GLfloat points[] = {
  -0.5f, -0.5f, -1.0f,
  -0.5f,  0.5f, -0.5f,
   0.5f, -0.5f,  0.5f,
   0.5f,  0.5f,  1.0f
};

static void key_callback(int key)
{
  // TODO: do something with the keys once we figure out function pointers.
}


void init_world()
{
  // Generate VBO for vertex attribs.
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // Generate VAO for vertex attribs.
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
}

void render_world() {
  points[1] = -0.5f + sinf((float)count * 0.1f) * 0.5f;
  points[10] = 0.5f + sinf((float)count * 0.1f) * 0.5f;
  points[0] = -0.5f + sinf((float)count * 0.1f) * 0.5f;
  points[9] = 0.5f + sinf((float)count * 0.1f) * 0.5f;

  points[4] = 0.5f + sinf((float)count * 0.1f) * 0.5f;
  points[7] = -0.5f + sinf((float)count * 0.1f) * 0.5f;
  points[3] = -0.5f + sinf((float)count * 0.1f) * 0.5f;
  points[6] = 0.5f + sinf((float)count * 0.1f) * 0.5f;
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
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

  Log_info("\nRunning app");
  Engine_print_hardware_info();
  Engine_log_fps();

  Engine_set_key_callback(key_callback);

  init_world();
  start_main_loop();

  Engine_print_program_log();
  return 0;
}
