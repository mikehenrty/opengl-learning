#version 100
attribute vec3 vertex;
varying vec2 texcoord;

void main()
{
  vec2 point = vec2(vertex.x, vertex.y);
  float corner = vertex.z;
  gl_Position = vec4(point, 1.0, 1.0);
  if (corner == -1.0) {
    texcoord = vec2(0.0, 0.0);
  } else if (corner == -0.5) {
    texcoord = vec2(0.0, 1.0);
  } else if (corner == 0.5) {
    texcoord = vec2(1.0, 0.0);
  } else if (corner == 1.0) {
    texcoord = vec2(1.0, 1.0);
  }
}
