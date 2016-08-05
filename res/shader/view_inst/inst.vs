#version 300 es

#define BASIC_ATTRIB_POS        0
#define BASIC_ATTRIB_NOR        1
#define BASIC_ATTRIB_INSTPOS    5

uniform mat4 worldMat, viewMat, projMat;

layout(location = BASIC_ATTRIB_POS) in vec3 position;
layout(location = BASIC_ATTRIB_NOR) in vec3 normal;
layout(location = BASIC_ATTRIB_INSTPOS) in mat4 perWorldMat;

out vec3 v_normal;

void main() {
    gl_Position = projMat * viewMat * perWorldMat * worldMat * vec4(position, 1.0);
    v_normal = normal;
}
