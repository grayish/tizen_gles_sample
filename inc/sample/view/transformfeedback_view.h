#ifndef __TRANSFORMFEEDBACK_VIEW__
#define __TRANSFORMFEEDBACK_VIEW__

#include "sample/sample_view.h"

class TransformParticles;

class TransformFeedbackView : public SampleView {
private:
	TransformParticles* mParticle;

public:
	/**
	 * @brief Constructor for the class TransformFeedbackView
	 *
	 * @param[in] data Data for GLView
	 */
	TransformFeedbackView(void *data);

	/**
	 * @brief Init function for the class TransformFeedbackView
	 */
	void OnInit() override;

	/**
	 * @brief Function called every frame
	 */
	void OnStep() override;

};


#endif // __TRANSFORMFEEDBACK_VIEW__
