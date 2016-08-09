#version 300 es

#define MRT_ATTRIB_POS      0
#define MRT_ATTRIB_NOR      1
#define MRT_ATTRIB_KA       2
#define MRT_ATTRIB_KD       3
#define MRT_ATTRIB_KS       4
#define MRT_ATTRIB_KE       5

uniform mat4 worldMat, viewMat, projMat;
uniform vec3 eyePos, lightPos;

layout(location = MRT_ATTRIB_POS) in vec3 position;
layout(location = MRT_ATTRIB_NOR) in vec3 normal;
layout(location = MRT_ATTRIB_KA ) in vec3 Ka;
layout(location = MRT_ATTRIB_KD ) in vec3 Kd;
layout(location = MRT_ATTRIB_KS ) in vec3 Ks;
layout(location = MRT_ATTRIB_KE ) in vec3 Ke;

out vec3 v_normal;
out vec3 v_lightDir, v_viewDir;
out vec3 v_Ka;
out vec3 v_Kd;
out vec3 v_Ks;
out vec3 v_Ke;
out float v_dist;

void main() {
    gl_Position = projMat * viewMat * worldMat * vec4(position, 1.0);
	v_normal = transpose(inverse(mat3(worldMat))) * normal;	
	
    vec3 posWS = (worldMat * vec4(position, 1.0)).xyz;
    v_lightDir = normalize(lightPos - posWS);
    v_viewDir = normalize(eyePos - posWS);
	
    v_Ka = Ka;
    v_Kd = Kd;
    v_Ks = Ks;
    v_Ke = Ke;
    v_dist = distance(posWS, lightPos);
}
