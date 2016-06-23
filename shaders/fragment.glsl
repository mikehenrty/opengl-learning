precision mediump float;

uniform sampler2D textures[2];

in vec2 texcoord;
flat in uint t_num;
layout(location = 0) out vec4 colorOut;

void main()
{
  if (t_num == uint(0)) {
    colorOut = texture(textures[0], texcoord);
  } else if (t_num == uint(1)) {
    colorOut = texture(textures[1], texcoord);
  }
}
