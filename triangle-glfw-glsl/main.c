#include "engine.h"

static const GLfloat points[] = {
  -0.5f, -0.5f, -1.0f,
  -0.5f,  0.5f, -0.5f,
   0.5f, -0.5f,  0.5f,
   0.5f,  0.5f,  1.0f
};

static void key_callback(int key) {
  // TODO: do something with the keys once we figure out function pointers.
}


void init_world()
{
  // Things main needs:
  GLuint vertex_buffer = 0;
  GLuint vertex_array = 0;

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

void start_main_loop()
{
  while (Engine_is_running()) {
    Engine_draw_everything();
    Engine_print_gl_error("Rendering");
  }
}


int main(int argc, char** argv)
{
  int result = Engine_init();
  if (!result) {
    return -1;
  }

  Engine_print_hardware_info();

  Engine_set_key_callback(key_callback);

  init_world();
  start_main_loop();

  Engine_print_program_log();
  return 0;
}
