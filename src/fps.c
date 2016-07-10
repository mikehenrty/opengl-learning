#include "gl.h"
#include "engine.h"
#include "logger.h"
#include "letter.h"
#include "fps.h"

// Time between FPS output in seconds.
static const float FPS_INTERVAL = 1.0;
static const unsigned LETTER_WIDTH = 20;
static const unsigned LETTER_HEIGHT = 30;
static const unsigned LETTER_COUNT = 5;

unsigned short int FPS_is_enabled = 0;

static Letter *letters[LETTER_COUNT];
static unsigned int frame_count = 0;
static double last_time = 0;

void FPS_enable()
{
  if (!FPS_is_enabled) {
    FPS_is_enabled = 1;

    float start_y = Engine_get_height() - LETTER_HEIGHT / 2 - 3;
    for (int i = 0; i < LETTER_COUNT; i++) {
      letters[i] = Letter_new(LETTER_WIDTH, LETTER_HEIGHT);
      float start_x = (LETTER_WIDTH / 2) + 3 + i * (LETTER_WIDTH + 3);
      Letter_set_position(letters[i], start_x, start_y);
    }
  }
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
    Letter_display(letters[0], (char)((int)average / 10) + '0');
    Letter_display(letters[1], (char)((int)average % 10) + '0');
    Letter_display(letters[2], '.');
    Letter_display(letters[3], (char)((int)(average * 10) % 10) + '0');
    Letter_display(letters[4], (char)((int)(average * 100) % 10) + '0');
    last_time = now;
    frame_count = 0;
  }
}

