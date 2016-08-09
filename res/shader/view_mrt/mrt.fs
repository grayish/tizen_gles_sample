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

out vec4 fragColor;

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
	sum += diff * S.diff * M.diff;  	// add diffuse term
	sum += spec * S.spec * M.spec;  	// add specular term
	sum += S.ambi * M.ambi + M.emit; 	// add ambient and emissive term

	return sum;
}

void main() {
	Material material =
		Material(materialSh, 
				 v_Kd, v_Ks, v_Ka, v_Ke);
	Light source =
		Light(normalize(v_lightDir), sourceDiff, sourceSpec, sourceAmbi);

    vec3 color = 
    	phongLight(normalize(v_viewDir), normalize(v_normal), material, source);

	fragColor = vec4(color, 1.0);
}