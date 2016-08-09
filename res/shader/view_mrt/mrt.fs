#version 300 es

precision mediump float;

uniform float materialSh;
uniform vec3 sourceDiff, sourceSpec, sourceAmbi;  // Sd, Ss, Sa

in vec3 v_normal;
in vec3 v_lightDir, v_viewDir;
in vec3 v_Ka;
in vec3 v_Kd;
in vec3 v_Ks;
in vec3 v_Ke;
in float v_dist;

layout(location = 0) out vec3 diffColor;
layout(location = 1) out vec3 drawColor;
layout(location = 2) out vec3 ambiColor;
layout(location = 3) out vec3 attnColor;

struct Material {
	float sh; 					 // shininess (specular power)
	vec3 diff, spec, ambi, emit; // material colors
};

struct Light {
	vec3 dir, diff, spec, ambi;  // light direction and colors
};

void phongLight(vec3 view, vec3 normal, // view direction and normal
				Material M, Light S,
				inout vec3 phong_diff,
				inout vec3 phong_spec,
				inout vec3 phong_ambi) {  // material and source light
	float diff = max(dot(normal, S.dir), 0.0);
	vec3 refl = 2.0 * normal * dot(normal, S.dir) - S.dir; // reflection vector
	float spec = 0.0;
	if(diff > 0.0) spec = pow(max(dot(refl, view), 0.0), M.sh);

	phong_diff = diff * S.diff * M.diff;  	// add diffuse term
	phong_spec = spec * S.spec * M.spec;  	// specular term
	phong_ambi = S.ambi * M.ambi + M.emit; 	// ambient and emissive term
}

void main() {
	Material material =
		Material(materialSh, 
				 v_Kd, v_Ks, v_Ka, v_Ke);
	Light source =
		Light(normalize(v_lightDir), sourceDiff, sourceSpec, sourceAmbi);

	vec3 c_diff, c_spec, c_ambi;
    phongLight(normalize(v_viewDir), normalize(v_normal), material, source,
    	c_diff,
    	c_spec,
    	c_ambi
    );
    float attenuation = 1.0 / (0.8 + 0.05 * v_dist + 0.02 * v_dist * v_dist);

	diffColor = v_Kd;
	ambiColor = v_Ka;
    attnColor = vec3(attenuation);
    drawColor = (c_diff + c_spec + c_ambi) * attenuation;
}