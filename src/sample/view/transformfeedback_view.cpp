#include "sample/view/transformfeedback_view.h"

#include "sample/noise.h"

using namespace std;

unsigned int Create3DNoiseTexture(int textureSize, float frequency) {
	GLuint textureId;
	GLfloat *texBuf = (GLfloat *) malloc(sizeof(GLfloat) * textureSize * textureSize * textureSize);
	GLubyte *uploadBuf = (GLubyte *) malloc(sizeof(GLubyte) * textureSize * textureSize * textureSize);
	int x, y, z;
	int index = 0;
	float min = 1000;
	float max = -1000;
	float range;

	initNoiseTable();

	for (z = 0; z < textureSize; z++) {
		for (y = 0; y < textureSize; y++) {
			for (x = 0; x < textureSize; x++) {
				float noiseVal;
				float pos[3] = {(float) x / (float) textureSize, (float) y / (float) textureSize,
								(float) z / (float) textureSize};
				pos[0] *= frequency;
				pos[1] *= frequency;
				pos[2] *= frequency;
				noiseVal = noise3D(pos);

				if (noiseVal < min) {
					min = noiseVal;
				}

				if (noiseVal > max) {
					max = noiseVal;
				}

				texBuf[index++] = noiseVal;
			}
		}
	}

	// Normalize to the [0, 1] range
	range = (max - min);
	index = 0;

	for (z = 0; z < textureSize; z++) {
		for (y = 0; y < textureSize; y++) {
			for (x = 0; x < textureSize; x++) {
				float noiseVal = texBuf[index];
				noiseVal = (noiseVal - min) / range;
				uploadBuf[index++] = (GLubyte) (noiseVal * 255.0f);
			}
		}
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_3D, textureId);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, textureSize, textureSize, textureSize, 0,
				 GL_RED, GL_UNSIGNED_BYTE, uploadBuf);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

	glBindTexture(GL_TEXTURE_3D, 0);

	free(texBuf);
	free(uploadBuf);

	return textureId;
}


class TransformRenderer : public BasicRenderer {
public:
	TransformRenderer() :
			BasicRenderer()
	{ }

	virtual ~TransformRenderer() {
	}

	virtual void RenderFrame() {
		ComputeTick();

		/*glBindFramebuffer(GL_FRAMEBUFFER, mShadowFbo);
		//set viewport size as a size of depth texture
		glViewport(0, 0, mShadowWidth, mShadowHeight);

		//disable color rendering
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		//clear depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);

		//Draw shadow
		Shader_Mgr.DrawObjects(SH_SHADOW_MAP);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, mWidth, mHeight);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		if (mIsAutoRotateEye) {
			mCamera->RotateAuto(mDeltaTime);
		}

		Object_Mgr.SetRotation(CalculateRot());

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		check_gl_error("glClear");

		Shader_Mgr.DrawObjects(SH_FRAG_SHADOW);*/

	}

};

TransformFeedbackView::TransformFeedbackView(void *data) :
		SampleView(data, false) {\
	mViewRenderer = new TransformRenderer();
}

void TransformFeedbackView::OnInit() {

	string vsParticle = File_Loader.ReadTxtFile("shader/view_tf/tf_particle.vs");
	string fsParticle = File_Loader.ReadTxtFile("shader/view_tf/tf_particle.fs");

	const char *feedbackVaryings[5] =
			{
					"v_position",
					"v_velocity",
					"v_size",
					"v_curtime",
					"v_lifetime"
			};

	/*InitTransformFeedback(vsParticle, fsParticle, feedbackVaryings);
	mNoiseTexId = Create3DNoiseTexture(128, 50.0);
	LOGI("3D Noise Texture ID : %d", mNoiseTexId);

	InitParticles();

	string vs = File_Loader.ReadTxtFile("shader/view_tf/tf_draw.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_tf/tf_draw.fs");

	TexProp tex(TEX_2D_FILE);
	File_Loader.ReadTexture("tex/smoke.tga", tex);*/


}