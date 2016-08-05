#version 300 es

precision mediump float;

uniform sampler2D s_tex0;

in vec3 v_normal;
in vec2 v_texCoord;

out vec4 fragColor;

void main() {
	//fragColor = vec4(v_texCoord.x, v_texCoord.y, 0.0, 1.0);
	fragColor = texture(s_tex0, v_texCoord);
}