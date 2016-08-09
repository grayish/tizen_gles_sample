#include "basic/obj/basic_light.h"

#include "basic/basic_container.h"
#include "basic/basic_shader.h"
#include "basic/obj/basic_camera.h"

using namespace std;
using namespace glm;

BasicLight::BasicLight(string name) :
		mName(name),
		mPosition(25.0f),
		mOrientation() {
	mOrientation = quat_cast(mat3(1));
}

BasicLight::~BasicLight() {

}

PointLight::PointLight(const string &name, const ABasicMap *list) :
		BasicLight(name),
		mDiff(0.7f),
		mSpec(1.0f),
		mAmbi(0.5f),
		mUniformList(dynamic_cast<const BasicMap<PointLt_U_Elem> *>(list)->mList) {}

PointLight::~PointLight() {}

void PointLight::SetLightUnforms(BasicShader *sh, BasicCamera *cam) {
	sh->SetUniformData(mUniformList[U_PL_DIFFUSE], mDiff);
	sh->SetUniformData(mUniformList[U_PL_AMBIENT], mAmbi);
	sh->SetUniformData(mUniformList[U_PL_SPECULAR], mSpec);
	sh->SetUniformData(mUniformList[U_PL_POS], mPosition);

	mat4 lightVp = glm::lookAt(mPosition, cam->GetAt(), cam->GetUp());
//    lightVp = glm::perspective(radians<float>(60.0f), 1.0f, 20.0f, 200.0f) * lightVp;
	lightVp = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 20.0f, 200.0f) * lightVp;
	sh->SetUniformData("lightVpMat", lightVp);
}