#ifndef ENGINE_H
#define ENGINE_H

#define MAX_SPRITES 5000
#define MAX_BACKGROUNDS 5

#include "sprite.h"

int  Engine_init(const char *name, int width, int height);
void Engine_set_key_callback(void *key_callback);
void Engine_print_hardware_info();
void Engine_print_program_log();
void Engine_print_gl_error(const char *message);
void Engine_draw_everything();
int  Engine_is_running();
void Engine_tick();
void Engine_show_fps();

int    Engine_get_width();
int    Engine_get_height();
double Engine_get_start_time();
double Engine_get_elapsed_time();

int  Engine_register_sprite(Sprite *sprite, const char *filename);
void Engine_update_sprite(Sprite *sprite);
int  Engine_get_texture_width(const char *filename);
int  Engine_get_texture_height(const char *filename);

int Engine_create_background(const char *filename, float pps);
int Engine_create_parallax_background(const char *filename, float pps,
                                      int height, float y);

#endif
