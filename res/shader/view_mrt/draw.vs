#version 300 es

#define BASIC_ATTRIB_POS        0
#define BASIC_ATTRIB_TEX        2

layout(location = BASIC_ATTRIB_POS) in vec2 pos;
layout(location = BASIC_ATTRIB_TEX) in vec2 tex;

out vec2 v_tex;

void main() {
	gl_Position = vec4(pos, 0.0, 1.0);
	v_tex = tex;
}