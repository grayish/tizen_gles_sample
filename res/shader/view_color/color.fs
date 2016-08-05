#version 300 es

precision mediump float;

in vec3 v_normal;
in vec2 v_texCoord;

out vec4 fragColor;

void main() {
	fragColor = vec4(0.5 * normalize(v_normal) + vec3(0.5), 1.0);
}
