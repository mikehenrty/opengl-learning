#include "gl.h"
#include "logger.h"
#include "fps.h"

// Time between FPS output in seconds.
static const float FPS_INTERVAL = 1.0;

unsigned short int FPS_is_enabled = 0;

static unsigned int frame_count = 0;
static double last_time = 0;

void FPS_enable()
{
  FPS_is_enabled = 1;
}

void FPS_tick()
{
  if (last_time == 0) {
    last_time = glfwGetTime();
  }

  ++frame_count;
  double now = glfwGetTime();
  double elapsed = now - last_time;
  if (elapsed > FPS_INTERVAL) {
    double average = frame_count / elapsed;
    Log_info("FPS: %f", average);
    last_time = now;
    frame_count = 0;
  }
}

