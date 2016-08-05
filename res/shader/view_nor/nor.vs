#version 300 es

#define BASIC_ATTRIB_POS        0
#define BASIC_ATTRIB_NOR        1
#define BASIC_ATTRIB_TEX        2
#define BASIC_ATTRIB_TANGENT    3

uniform mat4 worldMat, viewMat, projMat;
uniform vec3 eyePos, lightPos;

layout(location = BASIC_ATTRIB_POS) in vec3 position;
layout(location = BASIC_ATTRIB_NOR) in vec3 normal;
layout(location = BASIC_ATTRIB_TEX) in vec2 texCoord;
layout(location = BASIC_ATTRIB_TANGENT) in vec3 tangent;

out vec2 v_texCoord;
out vec3 v_lightDirTS;
out vec3 v_viewTS;

void main() {
    vec4 world_pos = worldMat * vec4(position, 1.0);
    gl_Position = projMat * viewMat * world_pos;
    v_texCoord = texCoord;

    vec3 nor = mat3(worldMat) * normal;
    vec3 tan = mat3(worldMat) * normalize(tangent);
    vec3 bin = cross(nor, tan);
    mat3 worldToTangentSpace = mat3(tan.x, bin.x, nor.x,
                                    tan.y, bin.y, nor.y,
                                    tan.z, bin.z, nor.z);

    v_lightDirTS = worldToTangentSpace * (lightPos - world_pos.xyz) ;
    v_viewTS = worldToTangentSpace * (eyePos - world_pos.xyz);

}


