#version 300 es

uniform mat4 worldMat, viewMat, projMat;
uniform vec3 eyePos, lightPos;
uniform mat4 lightVpMat;       // Light space

// Vertex attribute
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

// these will be pass to FS
out vec3 v_normal, v_lightDir, v_viewDir;
out vec4 v_shadowCoord;

// For calculating
const mat4 tMat = mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
        );

void main() {
    vec4 posWS = worldMat * vec4(position, 1.0);
    gl_Position = projMat * viewMat * posWS;

    v_normal = mat3(worldMat) * normal;
    v_lightDir = normalize(lightPos - posWS.xyz);
    v_viewDir = normalize(eyePos - posWS.xyz);

    // Light space coordinates modification
    v_shadowCoord = tMat * lightVpMat * posWS;
}

