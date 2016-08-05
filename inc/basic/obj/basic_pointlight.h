#ifndef __BASIC_POINTLIGHT_H__
#define __BASIC_POINTLIGHT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BasicPointlight {
private:
	glm::vec3 position;

	//light source terms
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;

public:
	/**
	 * @brief Constructor for class BasicPointlight
	 */
	BasicPointlight();

	/**
	 * @brief Get world matrix of the light
	 *
	 * @return World matrix of the light
	 */
	glm::mat4 GetWorldMat() const;

	/**
	 * @brief Get view matrix of the light
	 *
	 * @return View matrix of the light
	 */
	glm::mat4 GetViewMat() const;

	/**
	 * @brief Get projection matrix of the light
	 *
	 * @return Projection matrix of the light
	 */
	glm::mat4 GetPerspectiveMat() const;

	/**
	 * @brief Get MVP matrix of the light
	 *
	 * @return MVP matrix of the light
	 */
	glm::mat4 GetMvpMat() const;

	/**
	 * @brief Get position of the light
	 *
	 * @return Position of the light
	 */
	glm::vec3 GetPosition() const;

	/**
	 * @brief Get diffuse color of the light
	 *
	 * @return Diffuse color of the light
	 */
	glm::vec3 GetDiffuse() const;

	/**
	 * @brief Get specular color of the light
	 *
	 * @return Specular color of the light
	 */
	glm::vec3 GetSpecular() const;

	/**
	 * @brief Get ambient color of the light
	 *
	 * @return Ambient color of the light
	 */
	glm::vec3 GetAmbient() const;

	/**
	 * @brief set position of the light
	 *
	 * @param[in] pos Position vector to set
	 */
	void SetPosition(glm::vec3 pos);

	/**
	 * @brief set diffuse color of the light
	 *
	 * @param[in] dif Color vector to set
	 */
	void SetDiffuse(glm::vec3 dif);

	/**
	 * @brief set specular color of the light
	 *
	 * @param[in] spe Color vector to set
	 */
	void SetSpecular(glm::vec3 spe);

	/**
	 * @brief set ambient color of the light
	 *
	 * @param[in] amb Color vector to set
	 */
	void SetAmbient(glm::vec3 amb);

};

inline glm::mat4 BasicPointlight::GetWorldMat() const {
	return glm::translate(glm::mat4(1), position);
}

inline glm::mat4 BasicPointlight::GetViewMat() const {
	return glm::lookAt(position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
}

inline glm::mat4 BasicPointlight::GetPerspectiveMat() const {
	return glm::ortho(-10, 10, -10, 10, -30, 30);
}

inline glm::mat4 BasicPointlight::GetMvpMat() const {
	return GetPerspectiveMat() * GetViewMat() * GetWorldMat();
}

inline glm::vec3 BasicPointlight::GetPosition() const {
	return position;
}

inline glm::vec3 BasicPointlight::GetDiffuse() const {
	return diffuse;
}

inline glm::vec3 BasicPointlight::GetSpecular() const {
	return specular;
}

inline glm::vec3 BasicPointlight::GetAmbient() const {
	return ambient;
}

#endif
