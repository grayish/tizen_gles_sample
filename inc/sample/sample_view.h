#ifndef __SAMPLE_VIEW_H__
#define __SAMPLE_VIEW_H__

#include "basic/basic_renderer.h"
#include "basic/basic_file_loader.h"
#include "basic/basic_container.h"
#include "basic/obj/basic_object.h"
#include "basic/tex/tex_prop.h"
#include "basic/basic_gl_set.h"

class SampleView {
public:
	Evas_Object *GetGLView() const;

protected:
	Evas_Object *mGLView;
	BasicRenderer *mViewRenderer;

public:
	/**
	 * @brief Constructor for the class SampleView
	 *
	 * @param[in] data Data for GLView
	 * @param[in] createDefaultRenderer Determine to create a new renderer object
	 */
	SampleView(void *data, bool createDefaultRenderer = true);

	/**
	 * @brief Destructor for the class SampleView
	 */
	virtual ~SampleView();

	/**
	 * @brief Delete mViewRenderer
	 */
	void Release();

private:
	/**
	 * @brief Abstract init function for child class
	 */
	virtual void OnInit() = 0;

public:
	/**
	 * @brief Initialize SampleView and mViewRenderer
	 */
	void Initialize();

	/**
	 * @brief Abstract function called every frame
	 */
	virtual void OnStep();

	/**
	 * @brief Callback function called when the window size changed. Set viewport of an application
	 *
	 * @param[in] w Width of an window
	 * @param[in] h Height of an window
	 */
	void OnWindowUpdate(int w, int h) const;

	/**
	 * @brief set mTouchPoint in mViewRenderer
	 *
	 * @param[in] x x coordinate of touch point
	 * @param[in] y y coordinate of touch point
	 */
	void SetTouchPoint(float x, float y) const;

	/**
	 * @brief Set mTouch on and autorotation off in mViewRenderer
	 *
	 */
	void TouchOn() const;

	/**
	 * @brief Set mTouch off and autorotation on in mViewRenderer
	 *
	 */
	void TouchOff() const;

	/**
	 * @brief Zoom in to at point with a distance. The distance is up to a length between eye point and at point
	 *
	 * @param[in] vel Length to zoom in
	 */
	void ZoomIn(const float &vel) const;

	/**
	 * @brief Zoom out from at point with a distance
	 *
	 * @param[in] vel Length to zoom out
	 */
	void ZoomOut(const float &vel) const;

	/**
	 * @brief Move eye point forward in the direction of at point.
	 *
	 * @param[in] vel Length to move eye forward
	 */
	void MoveForward(const float &vel) const;

	/**
	 * @brief Move eye point back in the opposite direction of at point.
	 *
	 * @param[in] vel Length to move eye back
	 */
	void MoveBackward(const float &vel) const;

	/**
	 * @brief Move eye point right according to at point.
	 *
	 * @param[in] vel Length to move
	 */
	void MoveRight(const float &vel) const;

	/**
	 * @brief Move eye point left according to at point.
	 *
	 * @param[in] vel Length to move
	 */
	void MoveLeft(const float &vel) const;

	/**
	 * @brief Move eye up right according to at point.
	 *
	 * @param[in] vel Length to move
	 */
	void MoveUp(const float &vel) const;

	/**
	 * @brief Move eye point down according to at point.
	 *
	 * @param[in] vel Length to move
	 */
	void MoveDown(const float &vel) const;

	/**
	 * @brief Rotate camera around x- and y-axis
	 *
	 * @param[in] dx Angle to rotate camera around x-axis
	 * @param[in] dy Angle to rotate camera around y-axis
	 */
	void RotateCamera(const float &dx, const float &dy) const;

	/**
	 * @brief Create and setup glview
	 *
	 * @param[in] data Appdata
	 * @return a pointer of the glview
	 */
	Evas_Object *CreateView(void *data) const;
};

inline Evas_Object *SampleView::GetGLView() const {
	return mGLView;
}

#endif //__SAMPLE_VIEW_H__


