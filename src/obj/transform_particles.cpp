#include "basic/obj/transform_particles.h"

#include "basic/basic_utils.h"
#include "basic/basic_gl_set.h"

#include "basic/basic_shader.h"
#include "basic/mgr/basic_texture_mgr.h"


TransformParticles::TransformParticles(const std::string &name, const ABasicMap *list) :
	BasicObject(name),
	mColor(1.0f),
	mVertices(),
	mBufferVertices{0,},
	mSrcBuffer(0),
	mDstBuffer(0),
	mUniformList(),
	mSync()
{

}

TransformParticles::~TransformParticles() {
	LOGI("destruct name[%s]", mName.c_str());

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
	SetupAttribs();

	// Block the GL server until transform feedback results are completed
	glWaitSync ( mSync, 0, GL_TIMEOUT_IGNORED );
	glDeleteSync ( mSync );

	glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );
	check_gl_error("glDrawArrays");

	ResetAttrib();
}

void TransformParticles::SetShaderUniforms(BasicShader *sh) {
	sh->SetUniformData(mUniformList[U_EMISSION_RATE], EMISSION_RATE);
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



void TransformParticles::UpdateParticles(BasicShader *sh) {
	SetShaderUniforms(sh);
	sh->PassUniforms();
	sh->Use();

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

	// swap
	GLuint temp = mSrcBuffer;
	mSrcBuffer = mDstBuffer;
	mDstBuffer = temp;
}
