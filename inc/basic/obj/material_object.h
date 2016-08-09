#ifndef __BASIC_MATERIAL_OBJECT_H__
#define __BASIC_MATERIAL_OBJECT_H__

#include "basic_object.h"

#include <map>

class BasicObjectMgr;

class MaterialObject : public BasicObject {
	friend class BasicObjectMgr;

private:
	typedef std::map<MaterialObj_U_Elem, std::string> MaterialObj_U_Str;

	float mScale;

	// vertex data array and indices
	std::vector<MaterialVertex> mVertices;
	std::vector<GLushort> mIndices;

	// gl buffer obj and VAO
	GLuint mBufferVertices;
	GLuint mBufferIndices;
	GLuint mVertexArrayObject;
	MaterialObj_U_Str mUniformList;
	BasicCamera *mCamera;

public: //Getter Setter
	MaterialObject *SetScale(const float &scale);

public:
	MaterialObject(const std::string &name, const ABasicMap *list, BasicCamera *camera);

	virtual ~MaterialObject();

	virtual BasicObject *ImportObj(const std::string &objSource, const float &scale);

	BasicObject *ImporterScale(char *objSource, const float &scale);

	void ImporterMtl(char *mtlSource, std::map<std::string, Material> &mtlItems);

protected: // override functions
	virtual void CreateVbo();

	virtual void SetupAttribs();

	virtual void ResetAttrib();

	virtual void Draw();

	virtual void SetShaderUniforms(BasicShader *sh);
};

inline MaterialObject *MaterialObject::SetScale(const float &scale) {
	mScale = scale;
	return this;
}

#endif //__BASIC_MATERIAL_OBJECT_H__
