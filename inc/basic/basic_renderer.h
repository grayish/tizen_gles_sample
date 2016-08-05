#ifndef __BASIC_RENDERER_H__
#define __BASIC_RENDERER_H__

#include <vector>
#include <string>

#include "basic_utils.h"
#include "basic_type.h"

class BasicObject;

class BasicTimer;

class BasicCamera;

class BasicShader;

class SampleView;

class ABasicMap;

class BasicRenderer {
	friend class SampleView;

protected:
	BasicTimer *mTimer;
	BasicCamera *mCamera;
	BasicShader *mCurrShader;

	int mWidth;
	int mHeight;
	double mDeltaTime;
	double mTotalTime;

	bool mIsAutoRotateEye;
	bool mIsFill;

	bool mIsTouchOn;
	glm::vec2 mTouchPoint; // Stores Touch Point x, y determined by scrolling screen


public:
	/**
	 * @brief Constructor for class BasicRenderer
	 */
	BasicRenderer();

	/**
	 * @brief Destructor for class BasicRenderer
	 */
	virtual ~BasicRenderer();

	/// Renderer interface functions

	/**
	 * @brief Interface function for creating new object
	 *
     * @param[in] type type of the object
     * @param[in] type name name specified for the object
     *
     * @return a pointer of the object
	 */
	BasicObject *GetNewObject(BasicObject_Type type, const std::string &name, const ABasicMap &map);

	/**
	 * @brief Set viewport and compute perspective
	 *
	 * @param[in] w width of viewport
	 * @param[in] h height of viewport
	 *
	 */
	void SetViewPort(int w, int h);

	/**
     * @brief Initialize renderer
     */
	virtual void Initialize();

	/**
	 * @brief Render objects in the scene
	 *
	 */
	virtual void RenderFrame();

	void SetCurrShader(const std::string &sh_name);

protected:
	/**
	 * @brief Set basic options of GLES
	 *
	 */
	void SetStates() const;

	/**
	 * @brief Print information of GLES in log
	 *
	 */
	void LogInfo() const;

	/**
	 * @brief Update time since last ComputeTick() call
	 *
	 */
	void ComputeTick();

public: /// inline functions
	/**
	 * @brief Getter for mCamera
	 *
	 * @return mCamera
	 */
	BasicCamera *GetCamera();

	/**
	 * @brief set mTouchPoint
	 *
	 * @param[in] x x coordinate of touch point
	 * @param[in] y y coordinate of touch point
	 */
	void SetTouchPoint(const float &x, const float &y);

	/**
	 * @brief Set AutoRotation on
	 *
	 */
	void OnAutoRotate();

	/**
	 * @brief Set AutoRotation off
	 *
	 */
	void OffAutoRotate();

	/**
	 * @brief Toggle AutoRotation
	 *
	 */
	void ToggleRotate();

	/**
	 * @brief Set mTouch on
	 *
	 */
	void TouchOn();

	/**
	 * @brief Set mTouch off
	 *
	 */
	void TouchOff();

public:
	/**
	 * @brief Getter for mWidth
	 *
	 */
	int GetWidth();

	/**
	 * @brief Getter for mHeight
	 *
	 */
	int GetHeight();


	glm::vec3 GetArcballVector(const glm::vec2 &point) const;

	/**
	 * @brief set rotation of an arcball object
	 *
	 */
	glm::quat CalculateRot();

	/**
	 * @brief Attach an object as a scene's arcball object
	 *
	 * @param[in] obj object to set as an arcball
	 */
	//void SetFocus(BasicObject *obj);


};

inline BasicCamera *BasicRenderer::GetCamera() {
	return mCamera;
}

inline void BasicRenderer::SetTouchPoint(const float &x, const float &y) {
	mTouchPoint.x = x;
	mTouchPoint.y = y;
}

inline void BasicRenderer::OnAutoRotate() {
	mIsAutoRotateEye = true;
}

inline void BasicRenderer::OffAutoRotate() {
	mIsAutoRotateEye = false;
}

inline void BasicRenderer::ToggleRotate() {
	mIsAutoRotateEye = !mIsAutoRotateEye;
}

inline void BasicRenderer::TouchOn() {
	mIsTouchOn = true;
}

inline void BasicRenderer::TouchOff() {
	mIsTouchOn = false;
}

inline int BasicRenderer::GetWidth() {
	return mWidth;
}

inline int BasicRenderer::GetHeight() {
	return mHeight;
}

#endif //__BASIC_RENDERER_H__


