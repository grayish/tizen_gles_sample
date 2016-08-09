#ifndef __TRANSFORMFEEDBACK_VIEW__
#define __TRANSFORMFEEDBACK_VIEW__

#include "sample/sample_view.h"

class TransformParticles;

class TransformFeedbackView : public SampleView {
private:
	TransformParticles* mParticle;

public:
	TransformFeedbackView(void *data);

	void OnInit() override;

	void OnStep() override;

};


#endif // __TRANSFORMFEEDBACK_VIEW__
