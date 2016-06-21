#include <stdlib.h>
#include <string.h>

#include "gl.h"
#include "logger.h"
#include "loader.h"
#include "engine.h"

static GLFWwindow* window;
static unsigned int window_width;
static unsigned int window_height;
static GLuint program;

static void *external_key_callback;
static unsigned short int is_running = 0;

static const float FPS_INTERVAL = 1.0;
static unsigned short int log_fps = 0;

static void key_callback(GLFWwindow* window,
                         int key, int scancode,
                         int action, int mods)
{
  // TODO: figure out how to call *external_key_callback
  // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
  //   glfwSetWindowShouldClose(window, GL_TRUE);
  // }
  // For now, close the window with any key.
  glfwSetWindowShouldClose(window, GL_TRUE);
}

static void error_callback(int error, const char* description)
{
  Log("ERROR: %d, %s", error, description);
}

static char *get_shader_log(GLuint shader)
{
  GLint infoLogLength;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
  static char strInfoLog[10000];
  glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
  return &(strInfoLog[0]);
}

static void inc_fps()
{
  static unsigned int frame_count = 0;
  static double last_time = 0;

  if (last_time == 0) {
    last_time = glfwGetTime();
  }

  ++frame_count;
  double now = glfwGetTime();
  double elapsed = now - last_time;
  if (elapsed > FPS_INTERVAL) {
    double average = frame_count / elapsed;
    Log_info("FPS: %f", average);
    last_time = now;
    frame_count = 0;
  }
}




void Engine_print_hardware_info() {
  // Print graphics info.
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  Log_info("Renderer: %s", renderer);
  Log_info("OpenGL version supported %s\n", version);
}

void Engine_print_program_log() {
  if (!program) {
    Log("Cannot print program log when no program exists.");
    return;
  }

  int max_length = 2048;
  int actual_length = 0;
  char log[2048];
  glGetProgramInfoLog (program, max_length, &actual_length, log);
  if (actual_length > 0) {
    Log("program info log for GL index %u:\n%s", program, log);
  }
}

void Engine_print_gl_error(const char *message)
{
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    Log("%s::%d\n", message, err);
  }
}

void Engine_set_key_callback(void *key_callback) {
  external_key_callback = key_callback;
}

static GLFWwindow* init_window(int width, int height)
{
#ifdef __APPLE__
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  GLFWwindow* window = glfwCreateWindow(width, height, "", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 0;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  return window;
}

static GLuint init_shader(GLenum type, const char *filename)
{
  GLchar *version;
  GLchar *source;
  GLuint shader;
  GLint length, shader_ok;

  // My linux laptop only has 300es support.
#ifdef __APPLE__
  version = "#version 330\n";
#elif __linux__
  version = "#version 300 es\n";
#endif

  source = Loader_get_file_contents(filename, &length);
  if (!source) {
    Log("Failed to load shader file: %s\n", filename);
    return 0;
  }

  shader = glCreateShader(type);
  if (shader == 0) {
    Log("Could not create shader %d\n", type);
    return 0;
  }

  const char *sources[2] = { version, source };
  GLint lengths[2] = { strlen(version), length };
  glShaderSource(shader, 2, sources, lengths);
  free(source);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
  if (!shader_ok) {
    char *log = get_shader_log(shader);
    Log("Compilation error in shader %s: %s\n", filename, log);
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

static void init_shader_program()
{
  GLuint vertex_shader, fragment_shader;
  GLint program_ok;

  // TODO: stop the hardcoding, and allow the main file to specify shaders?
  vertex_shader = init_shader(GL_VERTEX_SHADER, "shaders/vertex.glsl");
  fragment_shader = init_shader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");
  if (vertex_shader == 0 || fragment_shader == 0) {
    Log("Could not init shaders\n");
  }

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
  if (!program_ok) {
    Log("Failed to link shader program.");
    Log("Vertex shader log: %s", get_shader_log(vertex_shader));
    Log("Fragment shader log: %s", get_shader_log(fragment_shader));
    Engine_print_program_log();
    glDeleteProgram(program);
  }
  glUseProgram(program);
}

static GLuint init_texture(const char *filename,
                           const char *name,
                           GLuint unit,
                           GLuint program)
{
  GLuint texture;
  int width, height;
  void *pixels = Loader_load_tga(filename, &width, &height);

  if (!pixels)
    return 0;

  glGenTextures(1, &texture);
  glActiveTexture(unit);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
  glTexImage2D(
      GL_TEXTURE_2D, 0,           /* target, level of detail */
      GL_RGB8,                    /* internal format */
      width, height, 0,           /* width, height, border */
      GL_BGR, GL_UNSIGNED_BYTE,   /* external format, type */
      pixels                      /* pixels */
      );
  free(pixels);
  GLint location = glGetUniformLocation(program, name);
  glUniform1i(location, 0);
  return texture;
}

int Engine_is_running() {
  return !glfwWindowShouldClose(window) && is_running;
}

void Engine_draw_everything() {
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glfwPollEvents();
  glfwSwapBuffers(window);
  if (log_fps) {
    inc_fps();
  }
}

void Engine_log_fps()
{
  log_fps = 1;
}

int Engine_get_width() {
  return window_width;
}

int Engine_get_height() {
  return window_height;
}

int Engine_init(int width, int height) {
  window_width = width;
  window_height = height;

  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    Log("Unable to initialize glfw.\n");
    return 0;
  }

  window = init_window(width, height);
  if (!window) {
    Log("Unable to initialize glfw window.\n");
    return 0;
  }

  glfwSetKeyCallback(window, key_callback);

  init_shader_program();
  if (!program) {
    Log("Unable to initialize shader program.\n");
    return 0;
  }

  // TODO: allow main program to specify multiple textures
  GLuint texture;
  texture = init_texture("data/hello2.tga", "textureHello", GL_TEXTURE0, program);
  if (texture == 0) {
    Log("Unable to initialize texture.");
    return 0;
  }

  is_running = 1;
  return 1;
}
