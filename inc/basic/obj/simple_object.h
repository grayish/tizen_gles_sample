#ifndef __SIMPLE_OBJECT_H__
#define __SIMPLE_OBJECT_H__

#include "basic/obj/basic_object.h"

#include "basic/basic_utils.h"
#include "basic/mgr/basic_texture_mgr.h"

typedef struct _SimpleVertex {
	glm::vec2 pos;
	glm::vec2 tex;
} SimpleVertex;

class BasicObjectMgr;

class SimpleObject : public BasicObject {
	friend class BasicObjectMgr;

private:
	std::vector<SimpleVertex> mVertices;

	// gl buffer obj and VAO
	GLuint mBufferVertices;

public:
	SimpleObject(const std::string &name, const ABasicMap *list, BasicCamera *camera) :
			BasicObject(name),
			mVertices(),
			mBufferVertices(0)
	{ }

	virtual ~SimpleObject() {
		LOGI("destruct name[%s]", mName.c_str());
		glDeleteBuffers(1, &mBufferVertices);
		check_gl_error("glDeleteBuffers");
	}

	virtual BasicObject *ImportObj(const std::string &objFilename, const float &scale) {
		return nullptr;
	}

protected:
	virtual void CreateVbo() {
		CreateBuffer(GL_ARRAY_BUFFER, &mBufferVertices,
					 (GLsizeiptr) mVertices.size() * sizeof(SimpleVertex),
					 &(mVertices.at(0)), GL_STATIC_DRAW);
	}

	virtual void SetupAttribs() {
		std::vector<VertexAttrib> att;
		att.push_back(VertexAttrib{BASIC_ATTRIB_POS, 2, GL_FLOAT,
								   sizeof(SimpleVertex),
								   (void *) offsetof(SimpleVertex, pos)});
		att.push_back(VertexAttrib{BASIC_ATTRIB_TEX, 2, GL_FLOAT,
								   sizeof(SimpleVertex),
								   (void *) offsetof(SimpleVertex, tex)});
		SetupVertexAttribs(mBufferVertices, att);


	}

	virtual void ResetAttrib() {

	}

	virtual void Draw() {
		SetupAttribs();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertices.size());

		ResetAttrib();

	}

	virtual void SetShaderUniforms(BasicShader *sh) {
		Texture_Mgr.ActiveTextures(sh, this);
	}

public:
	void Init(const glm::vec2& offset) {
		mVertices.push_back(
				SimpleVertex { glm::vec2(0.0, 0.0) + offset, glm::vec2(0.0, 0.0) } );
		mVertices.push_back(
				SimpleVertex { glm::vec2(0.0, 1.0) + offset, glm::vec2(0.0, 1.0) } );
		mVertices.push_back(
				SimpleVertex { glm::vec2(1.0, 0.0) + offset, glm::vec2(1.0, 0.0) } );
		mVertices.push_back(
				SimpleVertex { glm::vec2(1.0, 1.0) + offset, glm::vec2(1.0, 1.0) } );
	}

};


#endif //__SIMPLE_OBJECT_H__
