#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "engine.h"

static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

int Engine_init(void) {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    return -1;
  }
  return 1;
}
