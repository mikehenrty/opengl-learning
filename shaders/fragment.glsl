precision mediump float;

uniform sampler2D textures[16];

in vec2 texcoord;
flat in uint t_num;
layout(location = 0) out vec4 colorOut;

void main()
{
  if (t_num == uint(0)) {
    colorOut = texture(textures[0], texcoord);
  } else if (t_num == uint(1)) {
    colorOut = texture(textures[1], texcoord);
  } else if (t_num == uint(2)) {
    colorOut = texture(textures[2], texcoord);
  } else if (t_num == uint(3)) {
    colorOut = texture(textures[3], texcoord);
  } else if (t_num == uint(4)) {
    colorOut = texture(textures[4], texcoord);
  } else if (t_num == uint(5)) {
    colorOut = texture(textures[5], texcoord);
  } else if (t_num == uint(6)) {
    colorOut = texture(textures[6], texcoord);
  } else if (t_num == uint(7)) {
    colorOut = texture(textures[7], texcoord);
  } else if (t_num == uint(8)) {
    colorOut = texture(textures[8], texcoord);
  } else if (t_num == uint(9)) {
    colorOut = texture(textures[9], texcoord);
  } else if (t_num == uint(10)) {
    colorOut = texture(textures[10], texcoord);
  } else if (t_num == uint(11)) {
    colorOut = texture(textures[11], texcoord);
  } else if (t_num == uint(12)) {
    colorOut = texture(textures[12], texcoord);
  } else if (t_num == uint(13)) {
    colorOut = texture(textures[13], texcoord);
  } else if (t_num == uint(14)) {
    colorOut = texture(textures[14], texcoord);
  } else if (t_num == uint(15)) {
    colorOut = texture(textures[15], texcoord);
  }
}
