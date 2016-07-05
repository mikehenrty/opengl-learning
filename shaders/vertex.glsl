in vec4 vertex;
in uint t;
out vec2 texcoord;
flat out uint t_num;

void main()
{
  vec2 point = vec2(vertex.x, vertex.y);
  texcoord = vec2(vertex.z, vertex.w);
  gl_Position = vec4(point, 1.0, 1.0);
  t_num = t;
}
