#version 410

in vec2 point;
out vec2 texcoord;

void main()
{
  gl_Position = vec4(point, 1.0, 1.0);
  texcoord = vec2(point.x, point.y);
}
