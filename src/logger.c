#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

void Log_trace()
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  fprintf(stdout, "Obtained %zd stack frames.\n", size);
  for (i = 0; i < size; i++) {
    fprintf (stdout, "%s\n", strings[i]);
  }
  free(strings);
}
