#version 300 es

uniform mat4 mvpLightMatrix;

layout(location = 0) in vec4 a_position;
out vec4 v_color;

void main(){
	gl_Position = u_mvpLightMatrix * a_position;
}