precision mediump float;

uniform sampler2D textureHello;

in vec2 texcoord;
layout(location = 0) out vec4 colorOut;

void main()
{
  colorOut = texture(textureHello, texcoord);
}
