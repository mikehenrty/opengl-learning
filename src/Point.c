#include <math.h>

#include "engine.h"
#include "point.h"

static float pixel_to_gl_coordX(float pixelX)
{
  return (2.0f * (pixelX / Engine_get_width())) - 1;
}

static float pixel_to_gl_coordY(float pixelY)
{
  return (2.0f * (pixelY / Engine_get_height())) - 1;
}

void Point_rotate(Point *p, float angle, float center_x, float center_y)
{
  float s = sinf(angle);
  float c = cosf(angle);

  float x = p->x - center_x;
  float y = p->y - center_y;

  p->x = x * c - y * s + center_x;
  p->y = x * s + y * c + center_y;
}

void Point_to_gl_coords(Point *p)
{
  p->x = pixel_to_gl_coordX(p->x);
  p->y = pixel_to_gl_coordY(p->y);
}
