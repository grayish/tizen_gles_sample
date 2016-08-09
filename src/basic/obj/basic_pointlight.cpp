#include "basic/obj/basic_pointlight.h"

using namespace glm;

BasicPointlight::BasicPointlight() :
		position(vec3(50.0, 50.0, 50.0)),
		diffuse(vec3(0.7, 0.7, 0.7)),
		specular(vec3(1.0, 1.0, 1.0)),
		ambient(vec3(0.0, 0.0, 0.0)) {

}

void BasicPointlight::SetPosition(vec3 pos) {
	position = pos;
}

void BasicPointlight::SetDiffuse(vec3 dif) {
	diffuse = dif;
}

void BasicPointlight::SetSpecular(vec3 spe) {
	specular = spe;
}

void BasicPointlight::SetAmbient(vec3 amb) {
	ambient = amb;
}
