#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#endif

#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#include "logger.h"
#include "loader.h"
#include "engine.h"

// TODO: make these static (and therefore private)
GLFWwindow* window;
GLuint program;

static void *external_key_callback;
static int is_running = 0;

static void key_callback(GLFWwindow* window,
                         int key, int scancode,
                         int action, int mods)
{
  // TODO: figure out how to call *external_key_callback
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

static void error_callback(int error, const char* description)
{
  Log("ERROR: %d, %s", error, description);
}

void Engine_print_hardware_info() {
  // Print graphics info.
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  Log("Renderer: %s", renderer);
  Log("OpenGL version supported %s\n", version);
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

static GLFWwindow* init_window(void)
{
  #ifdef __APPLE__
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #endif
  GLFWwindow* window = glfwCreateWindow(640, 480, "Make it so", NULL, NULL);
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
  GLchar *source;
  GLuint shader;
  GLint length, shader_ok;

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

  glShaderSource(shader, 1, (const GLchar**)&source, &length);
  free(source);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
  if (!shader_ok) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    char strInfoLog[10000];
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
    Log("Compilation error in shader %s: %s\n", filename, strInfoLog);
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

static GLuint init_shader_program()
{
  GLuint program, vertex_shader, fragment_shader;
  GLint program_ok;

  // TODO: stop the hardcoding, and allow the main file to specify shaders?
  vertex_shader = init_shader(GL_VERTEX_SHADER, "shaders/vertex.glsl");
  fragment_shader = init_shader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");
  if (vertex_shader == 0 || fragment_shader == 0) {
    Log("Could not init shaders\n");
    return 0;
  }

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
  if (!program_ok) {
    Log("Failed tr link shader program:\n");
    Engine_print_program_log();
    glDeleteProgram(program);
    return 0;
  }
  glUseProgram(program);
  return program;
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
}

int Engine_init(void) {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    Log("Unable to initialize glfw.\n");
    return 0;
  }

  window = init_window();
  if (!window) {
    Log("Unable to initialize glfw window.\n");
    return 0;
  }

  glfwSetKeyCallback(window, key_callback);

  program = init_shader_program();
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
