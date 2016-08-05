#version 300 es

#define BASIC_ATTRIB_POS        0
#define BASIC_ATTRIB_NOR        1
#define BASIC_ATTRIB_TEX        2

uniform mat4 worldMat, viewMat, projMat;
uniform vec3 eyePos, lightPos;
uniform vec3 materialDiff, materialSpec, 		  // Md, Ms, Ma, Me
			 materialAmbi, materialEmit;
uniform float materialSh;
uniform vec3 sourceDiff, sourceSpec, sourceAmbi;  // Sd, Ss, Sa

layout(location = BASIC_ATTRIB_POS) in vec3 position;
layout(location = BASIC_ATTRIB_NOR) in vec3 normal;
layout(location = BASIC_ATTRIB_TEX) in vec2 texCoord;

out vec3 v_color;

struct Material {
	float sh; 					 // shininess (specular power)
	vec3 diff, spec, ambi, emit; // material colors
};

struct Light {
	vec3 dir, diff, spec, ambi;  // light direction and colors
};

vec3 phongLight(vec3 view, vec3 normal, // view direction and normal
				Material M, Light S) {  // material and source light
	float diff = max(dot(normal, S.dir), 0.0);
	vec3 refl = 2.0 * normal * dot(normal, S.dir) - S.dir; // reflection vector
	float spec = 0.0;
	if(diff > 0.0) spec = pow(max(dot(refl, view), 0.0), M.sh);

	vec3 sum = vec3(0.0);
	sum += diff * S.diff * M.diff; 		// add diffuse term
	sum += spec * S.spec * M.spec; 		// add specular term
	sum += S.ambi * M.ambi + M.emit; 	// add ambient and emissive term

	return sum;
}

void main() {
	gl_Position = projMat * viewMat * worldMat * vec4(position, 1.0);

	vec3 posWS = (worldMat * vec4(position, 1.0)).xyz;  // world space position
	vec3 norWS = mat3(worldMat) * normal; 		// uniform scale only
	vec3 lightDir = normalize(lightPos - posWS);
	vec3 viewDir = normalize(eyePos - posWS);

	Material material =
		Material(materialSh,
				 materialDiff, materialSpec, materialAmbi, materialEmit);
	Light source =
		Light(lightDir, sourceDiff, sourceSpec, sourceAmbi);

	v_color = phongLight(viewDir, norWS, material, source);
}


