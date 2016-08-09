#include "basic/obj/transform_particles.h"

#include "basic/basic_utils.h"

#include "basic/basic_shader.h"
#include "basic/basic_container.h"
#include "basic/mgr/basic_texture_mgr.h"
#include "basic/mgr/basic_shader_mgr.h"


TransformParticles::TransformParticles(const std::string &name, const ABasicMap *list) :
	BasicObject(name),
	mColor(1.0f),
	mVertices(),
	mBufferVertices{0,},
	mSrcBuffer(0),
	mDstBuffer(0),
	mUniformList(dynamic_cast<const BasicMap<TransformObj_U_Elem> *>(list)->mList),
	mSync(),
	mTime(0),
	mTransformShader(nullptr)
{
	InitParticle();
}

TransformParticles::~TransformParticles() {
	LOGI("destruct name[%s]", mName.c_str());
	glDeleteBuffers(2, mBufferVertices);
	check_gl_error("glDeleteBuffers");


}

void TransformParticles::CreateVbo() {
	CreateBuffer(GL_ARRAY_BUFFER, &mBufferVertices[0],
				 (GLsizeiptr) mVertices.size() * sizeof(Particle),
				 &(mVertices.at(0)), GL_DYNAMIC_COPY);

	CreateBuffer(GL_ARRAY_BUFFER, &mBufferVertices[1],
				 (GLsizeiptr) mVertices.size() * sizeof(Particle),
				 &(mVertices.at(0)), GL_DYNAMIC_COPY);

	mSrcBuffer = mBufferVertices[0];
	mDstBuffer = mBufferVertices[1];

}

void TransformParticles::SetupAttribs() {

	std::vector<VertexAttrib> att;
	att.push_back(VertexAttrib{TRANSFORM_ATTRIB_POSITION, 2, GL_FLOAT,
							   sizeof(Particle),
							   (void *) offsetof(Particle, pos)});
	att.push_back(VertexAttrib{TRANSFORM_ATTRIB_VELOCITY, 2, GL_FLOAT,
							   sizeof(Particle),
							   (void *) offsetof(Particle, vel)});
	att.push_back(VertexAttrib{TRANSFORM_ATTRIB_SIZE, 1, GL_FLOAT,
							   sizeof(Particle),
							   (void *) offsetof(Particle, size)});
	att.push_back(VertexAttrib{TRANSFORM_ATTRIB_CURTIME, 1, GL_FLOAT,
							   sizeof(Particle),
							   (void *) offsetof(Particle, curtime)});
	att.push_back(VertexAttrib{TRANSFORM_ATTRIB_LIFETIME, 1, GL_FLOAT,
							   sizeof(Particle),
							   (void *) offsetof(Particle, lifetime)});
	SetupVertexAttribs(mSrcBuffer, att);

}

void TransformParticles::ResetAttrib() {

}

void TransformParticles::Draw() {
//	UpdateParticles(mTransformShader);

	SetupAttribs();

	glEnable ( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA, GL_ONE );

	glWaitSync ( mSync, 0, GL_TIMEOUT_IGNORED );
	glDeleteSync ( mSync );

	glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );
	check_gl_error("glDrawArrays");

	ResetAttrib();
}

void TransformParticles::SetShaderUniforms(BasicShader *sh) {
	sh->SetUniformData(mUniformList[U_TIME], mTime);
	sh->SetUniformData(mUniformList[U_COLOR], mColor);
	sh->SetUniformData(mUniformList[U_ACCEL], glm::vec2(0, ACCELERATION));
	Texture_Mgr.ActiveTextures(sh, this);
}

void TransformParticles::InitParticle() {
	mVertices.reserve(NUM_PARTICLES);
	Particle p = {
			glm::vec2(0),
			glm::vec2(0),
			0, };
	for(int i=0; i<NUM_PARTICLES; i++) {
		mVertices.push_back(p);
	}
}

void TransformParticles::UpdateParticles() {
	mTime += 0.05f;
	mTransformShader->SetUniformData(mUniformList[U_EMISSION_RATE], EMISSION_RATE);
	mTransformShader->SetUniformData(mUniformList[U_TIME], mTime);
	Texture_Mgr.ActiveTextures(mTransformShader, this);

	mTransformShader->PassUniforms();
	mTransformShader->Use();

	SetupAttribs();

	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, mDstBuffer);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mDstBuffer);

	glEnable ( GL_RASTERIZER_DISCARD );

	glBeginTransformFeedback ( GL_POINTS );
	glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );
	glEndTransformFeedback();

	mSync = glFenceSync ( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );

	glDisable ( GL_RASTERIZER_DISCARD );
	glBindBufferBase ( GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0 );
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// swap
	GLuint temp = mSrcBuffer;
	mSrcBuffer = mDstBuffer;
	mDstBuffer = temp;
}

void TransformParticles::Attatch_TF_Shader(const std::string &vs, const std::string &fs, const char **varyings,
										   const std::string &name) {
	LOGI_ENTRY;
	mTransformShader = Shader_Mgr.GetNewShader(vs, fs, name);
	glTransformFeedbackVaryings(mTransformShader->GetProgram(), 5, varyings, GL_INTERLEAVED_ATTRIBS);
	check_gl_error("glTransformFeedbackVaryings");

	glLinkProgram(mTransformShader->GetProgram());
	check_gl_error("glLinkProgram");
	LOGI_EXIT;

}

BasicObject *TransformParticles::ImportObj(const std::string &objSource, const float &scale) {
	return nullptr;
}
