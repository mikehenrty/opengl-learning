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
  float x = p->x;
  float y = p->y;

  // translate point back to origin:
  x = x - center_x;
  y = y - center_y;

  // rotate point
  float xnew = x * c - y * s;
  float ynew = x * s + y * c;

  // translate point back:
  p->x = xnew + center_x;
  p->y = ynew + center_y;
}
void Point_to_gl_coords(Point *p)
{
  p->x = pixel_to_gl_coordX(p->x);
  p->y = pixel_to_gl_coordY(p->y);
}
