#version 300 es

precision mediump float;

layout(location = 0) out vec4 fragColor;

uniform vec4 u_color;
uniform sampler2D s_texture;

void main()
{
  vec4 texColor;
  texColor = texture( s_texture, gl_PointCoord );
  fragColor = texColor * u_color;
}
