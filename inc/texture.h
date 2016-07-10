#ifndef TEXTURE_H
#define TEXTURE_H

#define MAX_TEXTURES 16;

int Texture_get_index(const char *filename);
int Texture_create(const char *filename);
int Texture_get_width(const char *filename);
int Texture_get_height(const char *filename);

#endif
