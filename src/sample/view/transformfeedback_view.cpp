#include "sample/view/transformfeedback_view.h"

#include "sample/noise.h"

#define NUM_PARTICLES   200
#define EMISSION_RATE   0.3f
#define ACCELERATION   -1.0f

#define ATTRIBUTE_POSITION      0
#define ATTRIBUTE_VELOCITY      1
#define ATTRIBUTE_SIZE          2
#define ATTRIBUTE_CURTIME       3
#define ATTRIBUTE_LIFETIME      4

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


void SetupVertexAttributes(GLuint vboID) {
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glVertexAttribPointer(ATTRIBUTE_POSITION, 2, GL_FLOAT,
						  GL_FALSE, sizeof(Particle),
						  (const void *) NULL);

	glVertexAttribPointer(ATTRIBUTE_VELOCITY, 2, GL_FLOAT,
						  GL_FALSE, sizeof(Particle),
						  (const void *) offsetof (Particle, velocity[0]));

	glVertexAttribPointer(ATTRIBUTE_SIZE, 1, GL_FLOAT,
						  GL_FALSE, sizeof(Particle),
						  (const void *) offsetof (Particle, size));

	glVertexAttribPointer(ATTRIBUTE_CURTIME, 1, GL_FLOAT,
						  GL_FALSE, sizeof(Particle),
						  (const void *) offsetof (Particle, curtime));

	glVertexAttribPointer(ATTRIBUTE_LIFETIME, 1, GL_FLOAT,
						  GL_FALSE, sizeof(Particle),
						  (const void *) offsetof (Particle, lifetime));

	glEnableVertexAttribArray(ATTRIBUTE_POSITION);
	glEnableVertexAttribArray(ATTRIBUTE_VELOCITY);
	glEnableVertexAttribArray(ATTRIBUTE_SIZE);
	glEnableVertexAttribArray(ATTRIBUTE_CURTIME);
	glEnableVertexAttribArray(ATTRIBUTE_LIFETIME);
}

TransformFeedbackView::TransformFeedbackView(void *data) :
		SampleView(data, false),
		mParticleShader(nullptr),
		mParticleVbo{0,},
		mNoiseTexId(0) {
//	mParticleShader = new BasicShader("particle_sh");
}

TransformFeedbackView::~TransformFeedbackView() {
//	delete mParticleShader;
}

#define FileLoader FileLoader::Inst()

void TransformFeedbackView::OnInit() {

	string vsParticle = FileLoader.ReadTxtFile("shader/view_tfb/tfb_particle.vs");
	string fsParticle = FileLoader.ReadTxtFile("shader/view_tfb/tfb_particle.fs");

	const char *feedbackVaryings[5] =
			{
					"v_position",
					"v_velocity",
					"v_size",
					"v_curtime",
					"v_lifetime"
			};

	InitTransformFeedback(vsParticle, fsParticle, feedbackVaryings);
	mNoiseTexId = Create3DNoiseTexture(128, 50.0);
	LOGI("3D Noise Texture ID : %d", mNoiseTexId);

	InitParticles();

	string vs = FileLoader.ReadTxtFile("shader/view_nor/nor.vs");
	string fs = FileLoader.ReadTxtFile("shader/view_nor/nor.fs");

	string teapot = FileLoader.ReadTxtFile("obj3d/teapot");

	TexContainer tex;
	FileLoader.ReadTexture("tex/smoke.tga", tex);

	TexContainer normal;
	FileLoader.ReadTexture("tex/tex_n_brick.bmp", normal);

	BasicObject *obj;
	float scale = 1.0f;

/*	obj = mViewRenderer->SetNewModel(teapot, scale);
	obj->AttachShader(vs, fs);
	obj->SetTexture(tex);
	obj->SetNormalMapTexture(normal);

	mViewRenderer->Initialize();

	mViewRenderer->GetCamera()->SetEye(25, 25, 25);
	mViewRenderer->GetCamera()->SetAt(0, 0, 0);
	mViewRenderer->SetFocus(obj);*/


}

void TransformFeedbackView::OnStep() {
	EmitParticles(mParticleVbo[0], mParticleVbo[1]);
	mViewRenderer->RenderFrame();
}

void TransformFeedbackView::InitTransformFeedback(
		const string &vertexSource, const string &fragmentSource, const char **feedbackVaryings) {
//	mParticleShader->CreateProgram(vertexSource, fragmentSource);
//	// Set the vertex shader outputs as transform feedback varyings
//	glTransformFeedbackVaryings ( mParticleShader->GetProgram(), 5, feedbackVaryings, GL_INTERLEAVED_ATTRIBS );
//
//	// Link program must occur after calling glTransformFeedbackVaryings
//	glLinkProgram ( mParticleShader->GetProgram() );

}

void TransformFeedbackView::InitParticles() {

	Particle particleData[NUM_PARTICLES];
	// Initialize particle data
	memset(&particleData, 0, sizeof(Particle) * NUM_PARTICLES);

	// Create the particle VBOs
	glGenBuffers(2, &mParticleVbo[0]);

	for (int i = 0; i < 2; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, mParticleVbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM_PARTICLES, particleData, GL_DYNAMIC_COPY);
		LOGI("Particle VBO ID : %d", mParticleVbo[i]);
	}
}

void TransformFeedbackView::EmitParticles(GLuint srcVBO, GLuint dstVBO) {
//    mParticleShader->Use();

	SetupVertexAttributes(srcVBO);

	// Set transform feedback buffer
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, dstVBO);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, dstVBO);

	// Turn off rasterization - we are not drawing
	glEnable(GL_RASTERIZER_DISCARD);

	// Set uniforms
//	mParticleShader->SetUniform("u_time", 0.001f);
//	mParticleShader->SetUniform("u_emissionRate", EMISSION_RATE);

	// Bind the 3D noise texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, mNoiseTexId);
//	mParticleShader->SetUniform("s_noiseTex", 0);

	// Emit particles using transform feedback
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
	glEndTransformFeedback();

	// Create a sync obj to ensure transform feedback results are completed before the draw that uses them.
	mSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	// Restore state
	glDisable(GL_RASTERIZER_DISCARD);
	glUseProgram(0);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_3D, 0);

}
