#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "util.c"

static GLfloat points[] = {
  0.0f,  0.5f, 1.0f,
  0.5f, -0.5f, 1.0f,
  -0.5f, -0.5f, 1.0f
};

static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window,
                         int key, int scancode,
                         int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void print_program_log(GLuint programme) {
  int max_length = 2048;
  int actual_length = 0;
  char log[2048];
  glGetProgramInfoLog (programme, max_length, &actual_length, log);
  if (actual_length > 0) {
    printf ("program info log for GL index %u:\n%s", programme, log);
  }
}

void print_gl_error(const char *message)
{
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    fprintf(stderr, "%s::%d\n", message, err);
  }
}

static GLuint init_shader(GLenum type, const char *filename)
{
  GLchar *source;
  GLuint shader;
  GLint length, shader_ok;

  source = file_contents(filename, &length);
  if (!source) {
    fprintf(stderr, "Failed to load shader file: %s", filename);
    exit(EXIT_FAILURE);
  }

  shader = glCreateShader(type);
  if (shader == 0) {
    fprintf(stderr, "Could not create shader %d\n", type);
    exit(EXIT_FAILURE);
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
    fprintf(stderr, "Compilation error in shader %s: %s\n", filename, strInfoLog);
    glDeleteShader(shader);
    exit(EXIT_FAILURE);
  }
  return shader;
}

GLuint init_shader_program()
{
  GLuint program, vertex_shader, fragment_shader;
  GLint program_ok;

  vertex_shader = init_shader(GL_VERTEX_SHADER, "shaders/vertex.glsl");
  fragment_shader = init_shader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");
  if (vertex_shader == 0 || fragment_shader == 0) {
    fprintf(stderr, "Could not init shaders");
    exit(EXIT_FAILURE);
  }

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
  if (!program_ok) {
    fprintf(stderr, "Failed tr link shader program:\n");
    print_program_log(program);
    glDeleteProgram(program);
    exit(EXIT_FAILURE);
  }
  glUseProgram(program);
  return program;
}

GLuint init_texture(const char *filename,
                    const char *name,
                    GLuint unit,
                    GLuint program)
{
  GLuint texture;
  int width, height;
  void *pixels = read_tga(filename, &width, &height);

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

void init_world(GLuint program, GLuint vertex_buffer, GLuint vertex_array)
{
  GLuint texture;

  // textures
  texture = init_texture("data/hello2.tga", "textureHello", GL_TEXTURE0, program);
  if (texture == 0) {
    exit(EXIT_FAILURE);
  }

  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

GLFWwindow* init_window(void)
{
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Make it so", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSwapInterval(1);

  // Print graphics info.
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n\n", version);

  return window;
}

void init_glfw(void)
{
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
}



void start_main_loop(GLFWwindow* window, GLuint program,
                     GLuint vertex_buffer, GLuint vertex_array)
{

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    print_gl_error("Rendering");
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}


int main(int argc, char** argv)
{
  GLFWwindow* window;
  GLuint vertex_buffer = 0;
  GLuint vertex_array = 0;
  GLuint program;

  init_glfw();
  window = init_window();
  program = init_shader_program();
  init_world(program, vertex_buffer, vertex_array);
  start_main_loop(window, program, vertex_buffer, vertex_array);

  print_program_log(program);
  return 0;
}
