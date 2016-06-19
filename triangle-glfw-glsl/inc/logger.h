#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#define Log(M, ...) fprintf(stdout, "(%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif
