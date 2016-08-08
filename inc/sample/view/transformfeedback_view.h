#ifndef __TRANSFORMFEEDBACK_VIEW__
#define __TRANSFORMFEEDBACK_VIEW__

#include "sample/sample_view.h"

class BasicShader;

class TransformFeedbackView : public SampleView {
public:
	TransformFeedbackView(void *data);

	void OnInit() override;

};


#endif // __TRANSFORMFEEDBACK_VIEW__
