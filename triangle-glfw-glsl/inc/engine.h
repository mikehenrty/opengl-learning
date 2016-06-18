#ifndef ENGINE_H
#define ENGINE_H

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

int Engine_init();
void Engine_set_key_callback(void *key_callback);
void Engine_print_hardware_info();
void Engine_print_program_log();
void Engine_print_gl_error(const char *message);
void Engine_draw_everything();
int Engine_is_running();

// TODO: get rid of these global variables
GLFWwindow* window;
GLuint program;

#endif
