#include <glm/gtx/quaternion.hpp>

#include "basic/obj/basic_camera.h"

using namespace glm;

BasicCamera::BasicCamera() :
		mEye(25.0f),
		mDist(),
		mUp(0, 1.0f, 0.0f),
		mFw(0, 0, -1.0f),
		mAngle(0.0f, -45.0f, 0),
		m_zNear(2.0f),
		m_zFar(10000.0f),
		mAt(0),
		mPerspectiveMat() {

	mDist = distance(mEye, mAt);
	UpdateAngle();
}

BasicCamera::BasicCamera(const float &near, const float &far) :
		mEye(1.0f, 0, 0),
		mDist(),
		mUp(0, 1.0f, 0.0f),
		mFw(0, 0, -1.0f),
		mAngle(0.0f, -45.0f, 0),
		m_zNear(near),
		m_zFar(far),
		mAt(0),
		mPerspectiveMat() {
	mDist = distance(mEye, mAt);
	UpdateAngle();
}

void BasicCamera::UpdateAngle() {
	vec3 dir = mAt - mEye;
	vec3 projDir = projToPlane(dir, mUp);
	if (dot(dir, mUp) > 0) {
		mAngle.y = degrees(getAngle(projDir, dir)); // pitch
	} else {
		mAngle.y = degrees(-getAngle(projDir, dir));
	}
	mAngle.x = degrees(getAngle(mFw, projDir)); // yaw
}

void BasicCamera::UpdateAt() {
	vec3 dir = mFw;

	quat qYaw = angleAxis(radians(mAngle.x), mUp);
	dir = toMat3(qYaw) * dir;
	vec3 pitchAxis = cross(dir, mUp);
	quat qPitch = angleAxis(radians(mAngle.y), pitchAxis);
	dir = toMat3(qPitch) * dir;
	mAt = dir * mDist + mEye;
}

void BasicCamera::RotateAuto(const double &deltaTime) {
	float degree = (float) (ROTATION_VEL * deltaTime);

	// Add yaw angle;
	mAngle.x += degree;
	limitAngle(mAngle.x);

	vec3 prevAt = mAt;
	UpdateAt();

	vec3 deltaAt = prevAt - mAt;

	mAt = prevAt;
	mEye += deltaAt;
}

void BasicCamera::Rotate(const float &deltaX, const float &deltaY) {
	mAngle.x += deltaX;
	mAngle.y += deltaY;
	limitAngle(mAngle.x);
	mAngle.y = clamp<float>(mAngle.y, -89.9, 89.9);
	UpdateAt();
}

void BasicCamera::ZoomIn(const float &vel) {
	vec3 dir = mAt - mEye;
	float remain = length(dir);
	remain -= ZOOM_MIN_LENGTH;
	dir = normalize(dir);

	remain = (remain > vel) ? vel : remain;
	dir *= remain;
	mEye += dir;
	mDist -= remain;
}

void BasicCamera::ZoomOut(const float &vel) {
	vec3 dir = mEye - mAt;
	dir = normalize(dir);
	dir *= vel;
	mEye += dir;
	mDist += vel;
}


void BasicCamera::SetEye(const float &x, const float &y, const float &z) {
	mEye.x = x;
	mEye.y = y;
	mEye.z = z;
	mDist = distance(mEye, mAt);
	UpdateAngle();
}

void BasicCamera::SetEye(glm::vec3 eye) {
	mEye.x = eye.x;
	mEye.y = eye.y;
	mEye.z = eye.z;
	mDist = distance(mEye, mAt);
	UpdateAngle();
}

void BasicCamera::SetAt(const float &x, const float &y, const float &z) {
	mAt.x = x;
	mAt.y = y;
	mAt.z = z;
	mDist = distance(mEye, mAt);
	UpdateAngle();
}

void BasicCamera::SetAt(glm::vec3 at) {
	mAt.x = at.x;
	mAt.y = at.y;
	mAt.z = at.z;
	mDist = distance(mEye, mAt);
	UpdateAngle();
}

void BasicCamera::SetUp(const float &x, const float &y, const float &z) {
	mUp.x = x;
	mUp.y = y;
	mUp.z = z;
	UpdateAngle();
}

void BasicCamera::SetUp(glm::vec3 up) {
	mUp.x = up.x;
	mUp.y = up.y;
	mUp.z = up.z;
	UpdateAngle();
}

const glm::vec3 BasicCamera::GetEye() const {
	return mEye;
}

const glm::vec3 BasicCamera::GetAt() const {
	return mAt;
}

const glm::vec3 BasicCamera::GetUp() const {
	return mUp;
}

void BasicCamera::MoveForward(const float &vel) {
	vec3 dir = mAt - mEye;
	dir = normalize(dir) * vel;
	mEye += dir;
	UpdateAt();
}

void BasicCamera::MoveBackward(const float &vel) {
	vec3 dir = mEye - mAt;
	dir = normalize(dir) * vel;
	mEye += dir;
	UpdateAt();
}

void BasicCamera::MoveRight(const float &vel) {
	vec3 dir = mEye - mAt;
	dir = normalize(dir);
	dir = -cross(mUp, dir) * vel;
	mEye += dir;
	UpdateAt();
}

void BasicCamera::MoveLeft(const float &vel) {
	vec3 dir = mEye - mAt;
	dir = normalize(dir);
	dir = cross(mUp, dir) * vel;
	mEye += dir;
	UpdateAt();
}

void BasicCamera::MoveUp(const float &vel) {
	vec3 dir = mUp * vel;
	mEye += dir;
	UpdateAt();
}

void BasicCamera::MoveDown(const float &vel) {
	vec3 dir = -mUp * vel;
	mEye += dir;
	UpdateAt();
}

