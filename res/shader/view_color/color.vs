#version 300 es

#define BASIC_ATTRIB_POS        0
#define BASIC_ATTRIB_NOR        1
#define BASIC_ATTRIB_TEX        2

uniform mat4 worldMat, viewMat, projMat;

layout(location = BASIC_ATTRIB_POS) in vec3 position;
layout(location = BASIC_ATTRIB_NOR) in vec3 normal;
layout(location = BASIC_ATTRIB_TEX) in vec2 texCoord;

out vec3 v_normal;
out vec2 v_texCoord;

void main() {
    gl_Position = projMat * viewMat * worldMat * vec4(position, 1.0);
    v_normal = normal;
    v_texCoord = texCoord;
}
