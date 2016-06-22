in vec3 vertex;
out vec2 texcoord;

// Copied from sprite.h
#define LOWER_LEFT  -1.0f
#define UPPER_LEFT  -0.5f
#define LOWER_RIGHT  0.5f
#define UPPER_RIGHT  1.0f

void main()
{
  vec2 point = vec2(vertex.x, vertex.y);
  float corner = vertex.z;
  gl_Position = vec4(point, 1.0, 1.0);
  if (corner == LOWER_LEFT) {
    texcoord = vec2(0.0, 0.0);
  } else if (corner == UPPER_LEFT) {
    texcoord = vec2(0.0, 1.0);
  } else if (corner == LOWER_RIGHT) {
    texcoord = vec2(1.0, 0.0);
  } else if (corner == UPPER_RIGHT) {
    texcoord = vec2(1.0, 1.0);
  }
}
