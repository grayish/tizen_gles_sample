#ifndef __TRANSFORM_OBJECT_H__
#define __TRANSFORM_OBJECT_H__

#define NUM_PARTICLES   200
#define EMISSION_RATE   0.3f
#define ACCELERATION   -1.0f

#include "basic_object.h"

#include <map>

class BasicObjectMgr;

class TransformParticles : public BasicObject {
	friend class BasicObjectMgr;

private:
	typedef std::map<TransformObj_U_Elem, std::string> TransformObj_U_Str;

	glm::vec3 mColor;

	// vertex data array and indices
	std::vector<Particle> mVertices;

	// gl buffer obj and VAO
	GLuint mBufferVertices[2];
	GLuint mSrcBuffer;
	GLuint mDstBuffer;
	TransformObj_U_Str mUniformList;
	GLsync mSync;
	float mTime;

public: //Getter Setter

	TransformParticles *SetColor(const glm::vec3 &col);

public:
	TransformParticles(const std::string &name, const ABasicMap *list);

	virtual ~TransformParticles();

	void UpdateParticles(BasicShader *sh);

protected: // override functions
	virtual void CreateVbo();

	virtual void SetupAttribs();

	virtual void ResetAttrib();

	virtual void Draw();

	virtual void SetShaderUniforms(BasicShader *sh);

private:
	void InitParticle();

};

inline TransformParticles *TransformParticles::SetColor(const glm::vec3 &col) {
	mColor = col;
	return this;
}

#endif //__TRANSFORM_OBJECT_H__