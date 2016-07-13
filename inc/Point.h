#ifndef POINT_C
#define POINT_C

typedef struct Point {
  float x;
  float y;
} Point;

void Point_rotate(Point *p, float angle, float center_x, float center_y);
void Point_to_gl_coords(Point *p);

#endif
