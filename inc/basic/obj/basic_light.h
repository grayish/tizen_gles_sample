#ifndef __BASIC_LIGHT_H__
#define __BASIC_LIGHT_H__

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>

#include "basic/basic_type.h"

class BasicObject;

class BasicShader;

class BasicCamera;

class ABasicMap;

class BasicLight {
public:
	std::string mName;
	glm::vec3 mPosition;
	glm::quat mOrientation;

	BasicLight(std::string name);

	virtual ~BasicLight();

	virtual void SetLightUnforms(BasicShader *sh, BasicCamera *cam) = 0;
};

class PointLight : public BasicLight {
private:
	typedef std::map<PointLt_U_Elem, std::string> PL_UniformList;

	glm::vec3 mDiff;
	glm::vec3 mSpec;
	glm::vec3 mAmbi;
	PL_UniformList mUniformList;

public:
	PointLight(const std::string &name, const ABasicMap *list);

	virtual ~PointLight();

	virtual void SetLightUnforms(BasicShader *sh, BasicCamera *cam);
};

#endif //__BASIC_LIGHT_H__
