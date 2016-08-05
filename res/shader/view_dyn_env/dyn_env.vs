#version 300 es

uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_normal;
out vec2 v_texCoord;

void main() {
    gl_Position = projMat * viewMat * worldMat * vec4(position, 1.0);
    v_normal = mat3(worldMat) * normal;
    v_texCoord = texCoord;
}


