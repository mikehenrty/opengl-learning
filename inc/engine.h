#ifndef ENGINE_H
#define ENGINE_H

#define MAX_SPRITES 1000

#include "sprite.h"

int  Engine_init(int width, int height);
void Engine_set_key_callback(void *key_callback);
void Engine_print_hardware_info();
void Engine_print_program_log();
void Engine_print_gl_error(const char *message);
void Engine_draw_everything();
int  Engine_is_running();
void Engine_log_fps();

int    Engine_get_width();
int    Engine_get_height();
double Engine_get_start_time();
double Engine_get_elapsed_time();

int  Engine_register_sprite(Sprite *sprite);
void Engine_update_sprite(Sprite *sprite);

#endif
