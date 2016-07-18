#ifndef ENGINE_H
#define ENGINE_H

#define MAX_SPRITES 5000
#define MAX_TICK_CALLBACKS 5000

#include "logger.h"
#include "sprite.h"
#include "clouds.h"
#include "background.h"
#include "fps.h"

int  Engine_init(const char *name, int width, int height);
void Engine_kill();
void Engine_print_hardware_info();
void Engine_print_program_log();
void Engine_print_gl_error(const char *message);
void Engine_draw_everything();
int  Engine_is_running();

typedef void (*Engine_tick_callback)(void *obj, double elapsed, double since);
int  Engine_register_tick_callback(void *obj, Engine_tick_callback callback);
void Engine_tick();

typedef void (*Engine_key_callback)(int key);
void Engine_set_key_callback(Engine_key_callback key_callback);

int    Engine_get_width();
int    Engine_get_height();
double Engine_get_start_time();
double Engine_get_elapsed_time();

int  Engine_register_sprite(Sprite *sprite, const char *filename);
void Engine_update_sprite(Sprite *sprite);
int  Engine_get_texture_width(const char *filename);
int  Engine_get_texture_height(const char *filename);


#endif
