#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"
#include "logger.h"
#include "loader.h"

static short le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}

void *Loader_get_file_contents(const char *filename, int *length) {
    FILE *f = fopen(filename, "r");
    void *buffer;

    if (!f) {
        Log("Unable to open %s for reading", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(*length+1);
    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*)buffer)[*length] = '\0';

    return buffer;
}

void *Loader_load_tga(const char *filename, int *width, int *height) {
  struct tga_header {
    char  id_length;
    char  color_map_type;
    char  data_type_code;
    unsigned char  color_map_origin[2];
    unsigned char  color_map_length[2];
    char  color_map_depth;
    unsigned char  x_origin[2];
    unsigned char  y_origin[2];
    unsigned char  width[2];
    unsigned char  height[2];
    char  bits_per_pixel;
    char  image_descriptor;
  } header;
  int i, color_map_size, pixels_size;
  FILE *f;
  size_t read;
  void *pixels;

  f = fopen(filename, "rb");

  if (!f) {
    Log("Unable to open %s for reading", filename);
    return NULL;
  }

  read = fread(&header, 1, sizeof(header), f);

  if (read != sizeof(header)) {
    Log("%s has incomplete tga header", filename);
    fclose(f);
    return NULL;
  }
  if (header.data_type_code != 2) {
    Log("%s is not an uncompressed RGB tga file\n", filename);
    fclose(f);
    return NULL;
  }
  if (header.bits_per_pixel != 24) {
    Log("%s is not a 24-bit uncompressed RGB tga file", filename);
    fclose(f);
    return NULL;
  }

  for (i = 0; i < header.id_length; ++i)
    if (getc(f) == EOF) {
      Log("%s has incomplete id string", filename);
      fclose(f);
      return NULL;
    }

  color_map_size = le_short(header.color_map_length) * (header.color_map_depth/8);
  for (i = 0; i < color_map_size; ++i)
    if (getc(f) == EOF) {
      Log("%s has incomplete color map", filename);
      fclose(f);
      return NULL;
    }

  *width = le_short(header.width); *height = le_short(header.height);
  pixels_size = *width * *height * (header.bits_per_pixel/8);
  pixels = malloc(pixels_size);

  read = fread(pixels, 1, pixels_size, f);
  fclose(f);

  if (read != pixels_size) {
    Log("%s has incomplete image", filename);
    free(pixels);
    return NULL;
  }

  return pixels;
}

void *Loader_load_png(const char *filename, int *width, int *height)
{
  unsigned char *pixels;

  int error = lodepng_decode32_file(&pixels, (unsigned *)width, (unsigned *)height, filename);
  if (error) {
    Log("ERROR: loading png %s, %s", filename, lodepng_error_text(error));
    return NULL;
  }

  return pixels;
}
