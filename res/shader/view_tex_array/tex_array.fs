#version 300 es

precision mediump float;

uniform sampler2DArray s_texArray;
uniform float isTouch;

in vec2 v_texCoord;

out vec4 fragColor;

void main() {
	fragColor = texture(s_texArray, vec3(v_texCoord, isTouch));
}
