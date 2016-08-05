#ifndef __BASIC_PHONG_OBJECT_H__
#define __BASIC_PHONG_OBJECT_H__

#include "basic_object.h"

#include <map>

class BasicObjectMgr;

class PhongObject : public BasicObject {
	friend class BasicObjectMgr;

private:
	typedef std::map<PhongObj_U_Elem, std::string> PhongObj_U_Str;

	float mScale;
	glm::vec3 mDiffColor;
	glm::vec3 mSpecColor;
	glm::vec3 mAmbiColor;
	glm::vec3 mEmitColor;

	// vertex data array and indices
	std::vector<PhongVertex> mVertices;
	std::vector<Bushort> mIndices;

	// gl buffer obj and VAO
	Buint mBufferVertices;
	Buint mBufferIndices;
	Buint mVertexArrayObject;
	PhongObj_U_Str mUniformList;
	BasicCamera *mCamera;

public: //Getter Setter
	PhongObject *SetScale(const float &scale);

	PhongObject *SetColor(const glm::vec3 &col);

public:
	PhongObject(const std::string &name, const ABasicMap *list, BasicCamera *camera);

	virtual ~PhongObject();

	virtual BasicObject *ImportObj(const std::string &objSource, const float &scale);

	BasicObject *ImporterScale(char *objSource, const float &scale);

protected: // override functions
	virtual void CreateVbo();

	virtual void SetupAttribs();

	virtual void ResetAttrib();

	virtual void Draw();

	virtual void SetShaderUniforms(BasicShader *sh);

	void ComputeTangent();
};

inline PhongObject *PhongObject::SetColor(const glm::vec3 &col) {
	mDiffColor = col;
	return this;
}

inline PhongObject *PhongObject::SetScale(const float &scale) {
	mScale = scale;
	return this;
}

#endif //__BASIC_PHONG_OBJECT_H__