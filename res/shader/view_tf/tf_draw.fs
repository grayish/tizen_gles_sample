#version 300 es

precision mediump float;

layout(location = 0) out vec4 fragColor;

uniform vec3 u_color;
uniform sampler2D s_tex;

void main()
{
  vec4 texColor;
  texColor = texture( s_tex, gl_PointCoord );
  fragColor = texColor * vec4(u_color, 1.0);
//	fragColor = vec4(u_color, 1.0);
}
