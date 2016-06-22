#include <stdlib.h>

#include "logger.h"
#include "engine.h"
#include "sprite.h"

static GLuint vertex_array = 0;

static void print_points(Sprite *sprite)
{
  for (int i = 0; i < 12; i++) {
    Log_info("-- %f", sprite->points[i]);
  }
}

static float pixel_to_gl_coordX(float pixelX) {
  return (2.0f * (pixelX / Engine_get_width())) - 1;
}

static float pixel_to_gl_coordY(float pixelY) {
  return (2.0f * (pixelY / Engine_get_height())) - 1;
}

static void generate_points_from_position(Sprite *sprite)
{
  sprite->points[0] = pixel_to_gl_coordX(sprite->x - (sprite->width / 2));
  sprite->points[1] = pixel_to_gl_coordY(sprite->y - (sprite->height / 2));
  sprite->points[2] = LOWER_LEFT;

  sprite->points[3] = sprite->points[0];
  sprite->points[4] = pixel_to_gl_coordY(sprite->y + (sprite->height / 2));
  sprite->points[5] = UPPER_LEFT;

  sprite->points[6] = pixel_to_gl_coordX(sprite->x + (sprite->width / 2));
  sprite->points[7] = sprite->points[1];
  sprite->points[8] = LOWER_RIGHT;

  sprite->points[9] = sprite->points[6];
  sprite->points[10] = sprite->points[4];
  sprite->points[11] = UPPER_RIGHT;

  sprite->points[12] = sprite->points[0];
  sprite->points[13] = pixel_to_gl_coordY(sprite->y + (sprite->height / 2));
  sprite->points[14] = UPPER_LEFT;

  sprite->points[15] = pixel_to_gl_coordX(sprite->x + (sprite->width / 2));
  sprite->points[16] = sprite->points[1];
  sprite->points[17] = LOWER_RIGHT;
}

static void init(Sprite *sprite)
{
  // Generate VBO for vertex attribs.
  glGenBuffers(1, &sprite->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sprite->points), sprite->points, GL_STREAM_DRAW);

  // Generate VAO for vertex attribs.
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
}

Sprite* Sprite_new(int width, int height)
{
  Sprite *sprite = malloc(sizeof(Sprite));
  sprite->width = width;
  sprite->height = height;
  sprite->x = 0;
  sprite->y = 0;
  generate_points_from_position(sprite);
  init(sprite);
  return sprite;
}

void Sprite_render(Sprite *sprite)
{
  glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite->points), sprite->points);
}

void Sprite_set_position(Sprite *sprite, float x, float y)
{
  sprite->x = x;
  sprite->y = y;
  generate_points_from_position(sprite);
}
