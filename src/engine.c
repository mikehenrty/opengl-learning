#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gl.h"
#include "logger.h"
#include "loader.h"
#include "texture.h"
#include "fps.h"
#include "engine.h"

static GLFWwindow* window;
static unsigned int window_width;
static unsigned int window_height;
static GLuint program;

static void *external_key_callback;
static unsigned short int is_running = 0;
static double start_time = 0;

static int sprite_count = 0;
static Sprite *sprites[MAX_SPRITES];
static GLfloat sprite_points[MAX_SPRITES * SPRITE_SIZE] = {};
static GLuint texture_indices[MAX_SPRITES * SPRITE_NUM_INDICES * sizeof(int)];

static GLuint sprite_vbo = 0;
static GLuint sprite_vao = 0;
static GLuint texture_vbo = 0;

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

static int init_buffers()
{
  GLint sprite_location = glGetAttribLocation(program, "vertex");
  GLint texture_location = glGetAttribLocation(program, "t");
  if (sprite_location == -1 || texture_location == -1) {
    Log("ERROR: could not find attribute locations");
    return 0;
  }
  glGenBuffers(1, &sprite_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
  glGenVertexArrays(1, &sprite_vao);
  glBindVertexArray(sprite_vao);
  glVertexAttribPointer(sprite_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(sprite_location);
  glGenBuffers(1, &texture_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
  glVertexAttribIPointer(texture_location, 1, GL_UNSIGNED_INT, 0, 0);
  glEnableVertexAttribArray(texture_location);
  return 1;
}

static void update_buffers(int sprite_index)
{
  glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
  glBufferData(GL_ARRAY_BUFFER, SPRITE_SIZE * (sprite_index + 1),
               sprite_points, GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * SPRITE_NUM_INDICES * (sprite_index + 1),
               texture_indices, GL_STATIC_DRAW);
}

static int create_texture(const char *filename, int sprite_index)
{

  int texture_index = Texture_get_index(filename);
  if (texture_index == -1) {
    texture_index = Texture_create(filename);

    // Generate dynamic location for texture based on texture_index.
    char loc_str[64];
    sprintf(loc_str, "textures[%d]", texture_index);
    GLint location = glGetUniformLocation(program, loc_str);
    glUniform1i(location, texture_index);
  }

  Log_info("image: %s, index: %d", filename, texture_index);

  if (texture_index == -1) {
    Log("Unable to create new texture");
    return -1;
  }

  // Store our texture handle for later.
  int texture_index_offset = SPRITE_NUM_INDICES * sprite_index;
  for (int i = texture_index_offset;
       i < texture_index_offset + SPRITE_NUM_INDICES;
       i++) {
    texture_indices[i] = texture_index;
  }
  return texture_index;
}


void Engine_print_hardware_info()
{
  // Print graphics info.
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  Log_info("Renderer: %s", renderer);
  Log_info("OpenGL version supported %s\n", version);
}

void Engine_print_program_log()
{
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

static GLFWwindow* init_window(const char *name, int width, int height)
{
#ifdef __APPLE__
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
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

int Engine_is_running() {
  return !glfwWindowShouldClose(window) && is_running;
}

void Engine_draw_everything() {
  glDrawArrays(GL_TRIANGLES, 0, sprite_count * SPRITE_NUM_INDICES);
  glfwPollEvents();
  glfwSwapBuffers(window);
  if (FPS_is_enabled) {
    FPS_tick();
  }
}

void Engine_show_fps()
{
  FPS_enable();
}

int Engine_get_width() {
  return window_width;
}

int Engine_get_height() {
  return window_height;
}

double Engine_get_start_time()
{
  return start_time;
}

double Engine_get_elapsed_time()
{
  return glfwGetTime() - start_time;
}

int Engine_register_sprite(Sprite *sprite, const char *filename)
{
  if (sprite_count >= MAX_SPRITES) {
    Log("ERROR: unable to register new sprite, max reached.");
    return 0;
  }

  int sprite_index = sprite_count++;
  sprites[sprite_index] = sprite;

  if (create_texture(filename, sprite_index) == -1) {
    Log("Unable to link sprite and texture");
    return 0;
  }

  // Give the current sprite a spot in the sprite points array.
  sprite->points = &sprite_points[sprite_index * SPRITE_SIZE];
  update_buffers(sprite_index);
  return 1;
}

void Engine_update_sprite(Sprite *sprite)
{
  // Offset is the offset in the sprite point array times the sprite size.
  long offset = (sprite->points - &sprite_points[0]);
  glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, offset, SPRITE_SIZE, sprite->points);
}

int Engine_get_texture_width(const char *filename)
{
  return Texture_get_width(filename);
}

int Engine_get_texture_height(const char *filename)
{
  return Texture_get_height(filename);
}

void Engine_tick()
{
  double elapsed = Engine_get_elapsed_time();
  for (int i = 0; i < sprite_count; i++) {
    if (sprites[i]->animation_start > 0) {
      Sprite_tick(sprites[i], elapsed);
    }
  }
}

int Engine_init(const char *name, int width, int height) {
  window_width = width;
  window_height = height;

  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    Log("Unable to initialize glfw.");
    return 0;
  }

  window = init_window(name, width, height);
  if (!window) {
    Log("Unable to initialize glfw window.");
    return 0;
  }

  init_shader_program();
  if (!program) {
    Log("Unable to initialize shader program.");
    return 0;
  }

  // Turn on alpha channel for textures.
  // glEnable (GL_BLEND);
  // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (!init_buffers()) {
    Log("Unable to set up buffer objects");
    return 0;
  }
  glfwSetKeyCallback(window, key_callback);

  start_time = glfwGetTime();
  is_running = 1;
  return 1;
}
