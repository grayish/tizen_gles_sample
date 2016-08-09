#ifndef __TRANSFORMFEEDBACK_VIEW__
#define __TRANSFORMFEEDBACK_VIEW__

#include <basic/obj/transform_particles.h>
#include "sample/sample_view.h"

class BasicShader;

class TransformFeedbackView : public SampleView {
private:
	TransformParticles* mParticle;

public:
	TransformFeedbackView(void *data);

	void OnInit() override;

	void OnStep() override;

};


#endif // __TRANSFORMFEEDBACK_VIEW__
