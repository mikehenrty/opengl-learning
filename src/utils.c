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

/**
 * Returns true @odds percent of the time.
 */
int Utils_roll(float odds) {
  return (odds > Utils_random_int(1, 100)) ? 1 : 0;
}
