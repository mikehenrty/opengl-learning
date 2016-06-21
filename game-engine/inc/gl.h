#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glew.h>
#endif

