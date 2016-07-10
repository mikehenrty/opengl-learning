#include <stdlib.h>

#include "gl.h"
#include "logger.h"
#include "loader.h"
#include "hashmap.h"
#include "texture.h"

static int texture_count = 0;

typedef struct texture_metadata {
  int index;
  unsigned width;
  unsigned height;
} texture_metadata;

static map_t texture_map;

static texture_metadata* create_and_upload(const char *filename,
                                          int texture_index) {
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

  texture_metadata *metadata = malloc(sizeof(texture_metadata));
  metadata->width = width;
  metadata->height = height;
  metadata->index = texture_index;
  return metadata;
}

texture_metadata* get_metadata(const char *filename)
{
  if (!texture_map) {
    return 0;
  }

  // Check if we have already created a texture for this file.
  texture_metadata *metadata;
  int result = hashmap_get(texture_map, (char *)filename, (void **)(&metadata));
  if (result == MAP_OK) {
    return metadata;
  }

  // Texture metadata not found.
  return 0;
}


// Returns texture index, or -1 if it doesn't exist.
int Texture_get_index(const char *filename)
{
  texture_metadata *metadata = get_metadata(filename);
  if (metadata) {
    return metadata->index;
  }

  return -1;
}

// Returns texture width, or -1 if it doesn't exist.
int Texture_get_width(const char *filename)
{
  texture_metadata *metadata = get_metadata(filename);
  if (metadata) {
    return metadata->width;
  }

  return -1;
}
// Returns texture index, or -1 if it doesn't exist.
int Texture_get_height(const char *filename)
{
  texture_metadata *metadata = get_metadata(filename);
  if (metadata) {
    return metadata->height;
  }

  return -1;
}

int Texture_create(const char *filename)
{
  if (!texture_map) {
    texture_map = hashmap_new();
  }

  if (texture_count == MAX_TEXTURES) {
    Log("Unable to create texture, no more available texture units");
    return -1
  }

  // Create an index for new texture, and upload image to GPU.
  int texture_index = texture_count++;
  texture_metadata *metadata = create_and_upload(filename, texture_index);
  if (!metadata) {
    Log("Unable to create and upload texture");
    return -1;
  }

  // Store new texture index in the hashmap.
  int result = hashmap_put(texture_map, (char *)filename, metadata);
  if (result != MAP_OK) {
    Log("Unable to write texture to hashmap");
    return -1;
  }

  return texture_index;
}

