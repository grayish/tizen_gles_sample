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
	std::vector<Bushort> mIndices;
	std::vector<glm::mat4> mInstPosMat;

	// gl buffer obj and VAO
	Buint mBufferVertices;
	Buint mBufferIndices;
	Buint mBufferInstPos;

	Buint mVertexArrayObject;
	InstObj_U_Str mUniformList;
	BasicCamera *mCamera;

	float mScale;
	int mInstCount;

public:
	InstObject(const std::string &name, const ABasicMap *list, BasicCamera *camera);

	virtual ~InstObject();

	virtual BasicObject *ImportObj(const std::string &objSource, const float &scale);

	BasicObject *ImporterScale(char *objSource, const float &scale);

	void SetInstancing(const int &count, const int &row, const int &column, const float &gridSize);

protected:
	virtual void CreateVbo();

	virtual void SetupAttribs();

	virtual void ResetAttrib();

	virtual void Draw();

	virtual void SetShaderUniforms(BasicShader *sh);


};

#endif //__INSTANCED_OBJECT_H__