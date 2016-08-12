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

	/**
	 * @brief Constructor for the class BasicLight
	 *
	 * @param[in] name a name of the light
	 */
	BasicLight(std::string name);

	/**
	 * @brief Destructor for the class BasicLight
	 */
	virtual ~BasicLight();

	/**
	 * @brief Set uniform variables for the light
	 *
	 * @param[in] sh a pointer of a shader
	 * @param[in] cam a pointer of a camera
	 */
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
	/**
	 * @brief Constructor for the class PointLight
	 *
	 * @param[in] name a name of the light
	 * @param[in] name a map of the uniform variables
	 */
	PointLight(const std::string &name, const ABasicMap *list);

	/**
	 * @brief Destructor for the class PointLight
	 */
	virtual ~PointLight();

	/**
	 * @brief Set uniform variables for the light
	 *
	 * @param[in] sh a pointer of a shader
	 * @param[in] cam a pointer of a camera
	 */
	virtual void SetLightUnforms(BasicShader *sh, BasicCamera *cam);
};

#endif //__BASIC_LIGHT_H__
