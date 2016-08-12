#version 300 es

precision mediump float;

uniform samplerCube s_texCube;
uniform sampler2D s_tex0;
uniform vec3 eyePos;

in vec3 v_normal;
in vec2 v_texCoord;

layout(location = 0) out vec4 fragColor;

void main() {
	vec3 nor = normalize(v_normal);
	vec3 ray = normalize(eyePos);

	vec3 reflec = 2.0 * nor * dot(nor, ray) - ray;
	vec4 envColor = texture(s_texCube, reflec);

	vec4 col = 0.5 * texture(s_tex0, v_texCoord);
	fragColor = 0.5 * envColor + col;
//	fragColor = vec4(reflec, 1.0);

}