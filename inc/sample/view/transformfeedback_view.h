#ifndef __TRANSFORMFEEDBACK_VIEW__
#define __TRANSFORMFEEDBACK_VIEW__

#include "sample/sample_view.h"

class BasicShader;

typedef struct {
	float position[2];
	float velocity[2];
	float size;
	float curtime;
	float lifetime;
} Particle;

class TransformFeedbackView : public SampleView {
private:
	BasicShader *mParticleShader;
	GLuint mParticleVbo[2];
	GLuint mNoiseTexId;
	GLsync mSync;

public:
	TransformFeedbackView(void *data);

	~TransformFeedbackView();

	void OnInit() override;

	void OnStep() override;

	void InitTransformFeedback(const std::string &vertexSource, const std::string &fragmentSource,
							   const char **feedbackVaryings);

	void InitParticles();

	void EmitParticles(GLuint srcVBO, GLuint dstVBO);
};


#endif // __TRANSFORMFEEDBACK_VIEW__
