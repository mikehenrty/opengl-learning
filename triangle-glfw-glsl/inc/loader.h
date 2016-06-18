#ifndef LOADER_H
#define LOADER_H

void *Loader_get_file_contents(const char *filename, int *length);
void *Loader_load_tga(const char *filename, int *width, int *height);

#endif
