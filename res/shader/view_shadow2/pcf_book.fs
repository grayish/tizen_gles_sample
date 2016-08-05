#version 300 es

precision mediump float;

uniform sampler2DShadow s_texShadow; // shadow sampler

uniform vec3 materialDiff, materialSpec, 		  // Md, Ms, Ma, Me
			 materialAmbi, materialEmit;
uniform float materialSh;
uniform vec3 sourceDiff, sourceSpec, sourceAmbi;  // Sd, Ss, Sa

in vec3 v_normal, v_lightDir, v_viewDir;
in vec4 v_shadowCoord;

out vec4 fragColor;

struct Material {
	float sh; 			// shininess (specular power)
	vec3 d, s, a, e; 	// material colors : diffuse, specular, ambient, emission
};

struct Light {
	vec3 d, s, a;  		// light and colors : diffuse, specular, ambient
};

vec3 phongLight(vec3 v, vec3 n, vec3 l, // view direction, light direction and normal
				Material M, Light S) {  // material and source light
	// calculate diffuses term
	float diff = max(dot(n, l), 0.0);
	vec3 phong_diffuse = diff * S.d * M.d;

	// calculate specular term
	vec3 r = 2.0 * n * dot(n, l) - l; // reflection vector
	float spec = (diff > 0.0) ? pow(max(dot(r, v), 0.0), M.sh) : 0.0;
	vec3 phong_specular = spec * S.s * M.s;

	// join all terms together
	vec3 sum = phong_diffuse + phong_specular +  S.a * M.a + M.e;

	return sum;
}

// offset for shadow map. 1/witdh, 1/height
const float offsetX = 1.0 / 1024.0;
const float offsetY = 1.0 / 1024.0;

void main() {
	// Calculate visibility (3x3 PCF)
	// 3x3 kernel with 4 taps per sample, effectively 6x6 PCF
	float visibility = 0.0;
    float s, t;
    for(s = -2.0; s <= 2.0; s+=2.0) {
        for (t = -2.0; t<=2.0; t+=2.0) {
        	vec4 offset = vec4(s * offsetX, t * offsetY, -0.005, 0.0) * v_shadowCoord.w;
        	visibility += textureProj(s_texShadow, v_shadowCoord + offset);
    	}
    }
    visibility *= 0.111; // 1/9

	Material material =
		Material(materialSh,
				 materialDiff, materialSpec, materialAmbi, materialEmit);
	Light source =
		Light(sourceDiff, sourceSpec, sourceAmbi);

	vec3 color = phongLight(normalize(v_viewDir), normalize(v_normal), normalize(v_lightDir), material, source) * visibility;

	fragColor = vec4(color, 1.0);
//fragColor = vec4(m 1.0);
	//fragColor = vec4(vec3(visibility),1.0);

}
