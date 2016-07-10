#include <stdlib.h>
#include <time.h>

#include "utils.h"

int Utils_random_int(int min, int max)
{
  static int seed = 0;
  if (seed == 0) {
    seed = time(NULL);
    srand((unsigned)seed);
  }
  return rand() % (max - min) + min;
}
