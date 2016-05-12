#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include "util.c"

static struct {
  GLuint vertex_buffer, element_buffer;
  GLuint textures[2];

  /* ... fields for buffer and texture objects */
  GLuint vertex_shader, fragment_shader, program;

  struct {
    GLint fade_factor;
    GLint textures[2];
  } uniforms;

  struct {
    GLint position;
  } attributes;

  GLfloat fade_factor;
} g_resources;

static const GLfloat g_vertex_buffer_data[] = { 
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f
};

static const GLushort g_element_buffer_data[] = { 0, 1, 2, 3 };

static GLuint make_buffer(
    GLenum target,
    const void *buffer_data,
    GLsizei buffer_size
) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}

static GLuint make_texture(const char *filename) {
  GLuint texture;
  int width, height;
  void *pixels = read_tga(filename, &width, &height);

  if (!pixels)
    return 0;

  glGenTextures(1, &texture);
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
  return texture;
}

static GLuint make_shader(GLenum type, const char *filename)
{
  GLint length;
  GLchar *source = file_contents(filename, &length);
  GLuint shader;
  GLint shader_ok;

  if (!source) {
    return 0;
  }

  shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar**)&source, &length);
  free(source);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
  if (!shader_ok) {
    fprintf(stderr, "Failed to compile %s:\n", filename);
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

static GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)
{
  GLint program_ok;

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
  if (!program_ok) {
    fprintf(stderr, "Failed to link shader program:\n");
    glDeleteProgram(program);
    return 0;
  }
  return program;
}

static int make_resources(void) {
  // Making vertex and element buffers.
  g_resources.vertex_buffer = make_buffer(
    GL_ARRAY_BUFFER,
    g_vertex_buffer_data,
    sizeof(g_vertex_buffer_data)
  );
  g_resources.element_buffer = make_buffer(
    GL_ELEMENT_ARRAY_BUFFER,
    g_element_buffer_data,
    sizeof(g_element_buffer_data)
  );

  // Making textures.
  g_resources.textures[0] = make_texture("data/hello1.tga");
  g_resources.textures[1] = make_texture("data/hello2.tga");
  if (g_resources.textures[0] == 0 || g_resources.textures[1] == 0) {
    return 0;
  }

  // Make our shaders.
  g_resources.vertex_shader = make_shader(
      GL_VERTEX_SHADER,
      "shaders/vertex.glsl"
      );
  if (g_resources.vertex_shader == 0)
    return 0;

  g_resources.fragment_shader = make_shader(
      GL_FRAGMENT_SHADER,
      "shaders/fragment.glsl"
      );
  if (g_resources.fragment_shader == 0)
    return 0;

  // Make our shader program.
  g_resources.program = make_program(
      g_resources.vertex_shader,
      g_resources.fragment_shader
      );
  if (g_resources.program == 0)
    return 0;

  // Bind attributes and uniforms
  g_resources.uniforms.fade_factor
    = glGetUniformLocation(g_resources.program, "fade_factor");
  g_resources.uniforms.textures[0]
    = glGetUniformLocation(g_resources.program, "textures[0]");
  g_resources.uniforms.textures[1]
    = glGetUniformLocation(g_resources.program, "textures[1]");

  g_resources.attributes.position
    = glGetAttribLocation(g_resources.program, "position");

  return 1;
}

static void update_fade_factor(void) {
}

static void render(void) {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(400, 300);
  glutCreateWindow("Hello World");
  glutDisplayFunc(&render);
  glutIdleFunc(&update_fade_factor);
  if (!make_resources()) {
    fprintf(stderr, "Failed to load resources\n");
    return 1;
  }

  glutMainLoop();
  return 0;
}
