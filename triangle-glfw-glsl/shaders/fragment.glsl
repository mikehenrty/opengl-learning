#ifdef __linux__
precision mediump float;
#endif

uniform sampler2D textureHello;

in vec2 texcoord;
layout(location = 0) out vec4 colorOut;

void main()
{
  colorOut = texture(textureHello, texcoord);
}
