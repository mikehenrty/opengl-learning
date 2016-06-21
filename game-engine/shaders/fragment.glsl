#version 100
precision mediump float;

uniform sampler2D textureHello;

varying vec2 texcoord;

void main()
{
  gl_FragColor = texture2D(textureHello, texcoord);
}
