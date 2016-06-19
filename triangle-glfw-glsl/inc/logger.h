#ifndef LOGGER_H
#define LOGGER_H

#define Log(M, ...) fprintf(stdout, "(%s:%d) " M, __FILE__, __LINE__, ##__VA_ARGS__)

#endif
