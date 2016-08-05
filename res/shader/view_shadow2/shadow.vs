#version 300 es

layout(location = 0) in vec3 position;

uniform mat4 lightVpMat;
uniform mat4 worldMat;

void main() {
    gl_Position = lightVpMat * worldMat * vec4(position, 1.0);
}

