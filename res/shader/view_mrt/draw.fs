#version 300 es

precision mediump float;

uniform sampler2D s_tex;

in vec2 v_tex;

layout(location = 0) out vec4 fragColor;

void main() {
//	fragColor = vec4(1.0);
	fragColor = vec4(texture(s_tex, v_tex).xyz, 1.0);
}