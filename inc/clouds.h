#ifndef CLOUDS_H
#define CLOUDS_H

#define MAX_CLOUDS 30

int  Clouds_create(unsigned count);
void Clouds_tick(void * unused, double elapsed);

#endif
