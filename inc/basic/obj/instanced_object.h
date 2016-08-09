#ifndef __INSTANCED_OBJECT_H__
#define __INSTANCED_OBJECT_H__

#include <map>

#include "basic_object.h"

class BasicObjectMgr;


class InstObject : public BasicObject {
	friend class BasicObjectMgr;

private:
	typedef std::map<InstObj_U_Elem, std::string> InstObj_U_Str;

	// vertex data array and indices
	std::vector<InstVertex> mVertices;
	std::vector<GLushort> mIndices;
	std::vector<glm::mat4> mInstPosMat;

	// gl buffer obj and VAO
	GLuint mBufferVertices;
	GLuint mBufferIndices;
	GLuint mBufferInstPos;

	GLuint mVertexArrayObject;
	InstObj_U_Str mUniformList;
	BasicCamera *mCamera;

	float mScale;
	int mInstCount;

public:
	InstObject(const std::string &name, const ABasicMap *list, BasicCamera *camera);

	virtual ~InstObject();

	virtual BasicObject *ImportObj(const std::string &objFilename, const float &scale);

	void SetInstancing(const int &count, const int &row, const int &column, const float &gridSize);

protected:
	virtual void CreateVbo();

	virtual void SetupAttribs();

	virtual void ResetAttrib();

	virtual void Draw();

	virtual void SetShaderUniforms(BasicShader *sh);


};

#endif //__INSTANCED_OBJECT_H__
