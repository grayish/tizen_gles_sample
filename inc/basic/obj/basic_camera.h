#ifndef __BASIC_CAMERA_H__
#define __BASIC_CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BasicCamera {
private:
	const float ROTATION_VEL = 45.0f;
	const float ZOOM_MIN_LENGTH = 5.0f;

	glm::vec3 mEye;
	float mDist;
	glm::vec3 mUp;
	glm::vec3 mFw;
	glm::vec3 mAngle; // yaw pitch, roll

	float m_zNear;
	float m_zFar;

	glm::vec3 mAt;

	glm::mat4 mPerspectiveMat;

public:
	/**
	 * @brief Constructor for the class BasicCamera
	 */
	BasicCamera();

	BasicCamera(const float &near, const float &far);

	/**
	 * @brief Get view matrix that are currently set
	 *
	 * @return View matrix
	 */
	glm::mat4 GetViewMat() const;

	/**
	 * @brief Get projection matrix that are currently set
	 *
	 * @return Projection matrix
	 */
	glm::mat4 GetPerspectiveMat() const;

	/**
	 * @brief Set projection matrix
	 *
	 * @param[in] mat Matrix to set
	 */
	void SetPerspectiveMat(glm::mat4 mat);

	/**
	 * @brief Compute new projection matrix using screen size and field of view
	 *
	 * @param[in] fovy Field of view
	 * @param[in] w Width of screen
	 * @param[in] w Height of screen
	 */
	void ComputePerspective(const float &fovy, const int &w, const int &h);

	/**
	 * @brief Get eye vector
	 *
	 * @return Eye vector of view matrix
	 */
	const glm::vec3 GetEye() const;

	/**
	 * @brief Get at vector
	 *
	 * @return At vector of view matrix
	 */
	const glm::vec3 GetAt() const;

	/**
	 * @brief Get up vector
	 *
	 * @return Up vector of view matrix
	 */
	const glm::vec3 GetUp() const;

	/**
	 * @brief Set eye vector
	 *
	 * @param[in] x x value of eye vector
	 * @param[in] y y value of eye vector
	 * @param[in] z z value of eye vector
	 */
	void SetEye(const float &x, const float &y, const float &z);

	/**
	 * @brief Set eye vector
	 *
	 * @param[in] eye Vector set to view matrix
	 */
	void SetEye(glm::vec3 eye);

	/**
	 * @brief Set at vector
	 *
	 * @param[in] x x value of at vector
	 * @param[in] y y value of at vector
	 * @param[in] z z value of at vector
	 */
	void SetAt(const float &x, const float &y, const float &z);

	/**
	 * @brief Set at vector
	 *
	 * @param[in] At vector set to view matrix
	 */
	void SetAt(glm::vec3 at);

	/**
	 * @brief Set up vector
	 *
	 * @param[in] x x value of up vector
	 * @param[in] y y value of up vector
	 * @param[in] z z value of up vector
	 */
	void SetUp(const float &x, const float &y, const float &z);

	/**
	 * @brief Set up vector
	 *
	 * @param[in] up Vector set to view matrix
	 */
	void SetUp(glm::vec3 up);

	/**
	 * @brief Rotate camera around x-axis with constant angular speed
	 *
	 * @param[in] deltaTime Time for rotating camera
	 */
	void RotateAuto(const double &deltaTime);

	/**
	 * @brief Rotate camera around x- and y-axis
	 *
	 * @param[in] deltaX Angle to rotate camera around x-axis
	 * @param[in] deltaY Angle to rotate camera around y-axis
	 */
	void Rotate(const float &deltaX, const float &deltaY);

	/**
	 * @brief Zoom in to at point with a distance. The distance is up to a length between eye point and at point
	 *
	 * @param[in] vel Length to zoom in
	 */
	void ZoomIn(const float &vel);

	/**
	 * @brief Zoom out from at point with a distance
	 *
	 * @param[in] vel Length to zoom out
	 */
	void ZoomOut(const float &vel);

	/**
	 * @brief Move eye point forward in the direction of at point.
	 *
	 * @param[in] vel Length to move eye forward
	 */
	void MoveForward(const float &vel);

	/**
	 * @brief Move eye point back in the opposite direction of at point.
	 *
	 * @param[in] vel Length to move eye back
	 */
	void MoveBackward(const float &vel);

	/**
	 * @brief Move eye point right according to at point.
	 *
	 * @param[in] vel Length to move
	 */
	void MoveRight(const float &vel);

	/**
	 * @brief Move eye point left according to at point.
	 *
	 * @param[in] vel Length to move
	 */
	void MoveLeft(const float &vel);

	/**
	 * @brief Move eye up right according to at point.
	 *
	 * @param[in] vel Length to move
	 */
	void MoveUp(const float &vel);

	/**
	 * @brief Move eye point down according to at point.
	 *
	 * @param[in] vel Length to move
	 */
	void MoveDown(const float &vel);

	/**
	 * @brief Update eye vector of view matrix
	 */
	void UpdateAt();

	/**
	 * @brief Update angles of camera
	 */
	void UpdateAngle();
};

inline glm::vec3 projToVector(const glm::vec3 &target, const glm::vec3 &axis) {
	float numerator = glm::dot(target, axis);
	float denom = glm::length(axis);
	denom *= denom;
	denom = 1.0f / denom;
	return numerator * denom * axis;
}

inline glm::vec3 projToPlane(const glm::vec3 &target, const glm::vec3 &normal) {
	return target - projToVector(target, normal);
}

inline float getAngle(const glm::vec3 &a, const glm::vec3 &b) {
	return glm::acos(glm::dot(glm::normalize(a), glm::normalize(b)));
}


inline void limitAngle(float &angle) {
	if (angle > 180.0f) angle -= 360.0f;
	else if (angle < -180.0f) angle += 360.0f;
}

inline glm::mat4 BasicCamera::GetViewMat() const {
	return glm::lookAt(mEye, mAt, mUp);
}

inline void BasicCamera::SetPerspectiveMat(glm::mat4 mat) {
	mPerspectiveMat = mat;
}

inline glm::mat4 BasicCamera::GetPerspectiveMat() const {
	return mPerspectiveMat;
}

inline void BasicCamera::ComputePerspective(const float &fovy, const int &w, const int &h) {
	mPerspectiveMat = glm::perspective(fovy, static_cast<float>(w) / static_cast<float>(h), m_zNear, m_zFar);
}

#endif //__BASIC_CAMERA_H__


