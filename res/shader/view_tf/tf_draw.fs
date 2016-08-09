#version 300 es

precision mediump float;

uniform sampler2D s_tex;

in vec4 v_color;

layout(location = 0) out vec4 fragColor;

void main()
{
  vec4 texColor = texture( s_tex, gl_PointCoord );
  fragColor = texColor * v_color;
}
