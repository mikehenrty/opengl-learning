#include <stdlib.h>

#include "gl.h"
#include "logger.h"
#include "loader.h"
#include "hashmap.h"
#include "texture.h"

static int texture_count = 0;

typedef struct hashmap_value {
  int texture_index;
} hashmap_value;

static map_t texture_map;

static int create_and_upload(const char *filename, int texture_index) {
  GLuint texture;

  // Load the image file from disk. For now we assume TGA
  int width, height;
  void *pixels = Loader_load_png(filename, &width, &height);
  if (!pixels) {
    Log("ERROR: unable to load file %s", filename);
    return 0;
  }

  // Upload texture to GPU.
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0 + texture_index);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
  glTexImage2D(
      GL_TEXTURE_2D, 0,           /* target, level of detail */
      GL_RGBA,                    /* internal format */
      width, height, 0,           /* width, height, border */
      GL_RGBA, GL_UNSIGNED_BYTE,   /* external format, type */
      pixels                      /* pixels */
      );
  free(pixels);

  return 1;
}

// Returns texture index, or -1 if it doesn't exist.
int Texture_get_index(const char *filename)
{
  if (!texture_map) {
    return -1;
  }

  // Check if we have already created a texture for this file.
  hashmap_value *value;
  int result = hashmap_get(texture_map, (char *)filename, (void **)(&value));
  if (result == MAP_OK) {
    return value->texture_index;
  }

  // Texture not found.
  return -1;
}

int Texture_create(const char *filename)
{
  if (!texture_map) {
    texture_map = hashmap_new();
  }

  // Create an index for new texture, and upload image to GPU.
  int texture_index = texture_count++;
  if (!create_and_upload(filename, texture_index)) {
    Log("Unable to create and upload texture");
    return -1;
  }

  // Store new texture index in the hashmap.
  hashmap_value *value = malloc(sizeof(hashmap_value));
  value->texture_index = texture_index;
  int result = hashmap_put(texture_map, (char *)filename, value);
  if (result != MAP_OK) {
    Log("Unable to write texture to hashmap");
    return -1;
  }

  return texture_index;
}

