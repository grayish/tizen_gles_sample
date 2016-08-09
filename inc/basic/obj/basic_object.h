#ifndef __BASIC_OBJECT_H__
#define __BASIC_OBJECT_H__

#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>

#include "basic/basic_type.h"

class BasicLight;

class TexProp;

class BasicCamera;

class ABasicMap;

class BasicShaderMgr;

class BasicShader;

class BasicObject { // pure virtual class
	friend class BasicObjectMgr;

	friend class BasicShaderMgr;

protected:
	// basic property
	std::string mName;    //name of an obj
	glm::vec3 mPosition;
	glm::quat mOrientation;
	bool mFocus;
	bool mIsSelected;
	bool mIsVisible;

public: // inline getter and setter
	BasicObject *SetPosition(const glm::vec3 &pos);

	void SetRotation(const glm::quat &q);

	glm::quat GetRotation();

	void SetFocus(const bool &focus);

	void Select(const bool &isSelected);

	void SetVisible(const bool &isVisible);

public:
	BasicObject(const std::string &name);

	virtual ~BasicObject();

	/**
	 * @brief compile a shader and attach it to the object
	 *
	 * @param[in] vertexSource The source code of the vertex shader
	 * @param[in] fragmentSource The source code of the fragment shader
	 * @return this object
	 */
	BasicObject *
	AttachShader(const std::string &vertexSource, const std::string &fragmentSource, const std::string &name);

	BasicObject *AttachShader(const std::string &name);

	BasicObject *AttachLight(BasicLight_Type type, const std::string &name, const ABasicMap &map);

	BasicObject *AttachLight(const std::string &name);

	BasicObject *AttachTexture(const TexProp &tex, const std::string &uniform_name);

//	BasicObject *AttachCubeTex(const TexProp *cubeTex, const std::string &uniform_name);

	/**
	 * @brief set uniform in basic shader
	 */
	virtual void SetShaderUniforms(BasicShader *sh) = 0;

	/// pure virtual functions
	/**
 	 * @brief import obj file
 	 */
	virtual BasicObject *ImportObj(const std::string &objFilename, const float &scale) =0 ;

protected:
	/**
	 * @brief create buffer objects and bind buffer data
	 */
	virtual void CreateVbo() = 0;

	/**
	 * @brief Enable vertex attribute pointers and active textures for drawing, binding data for each vertex
	 */
	virtual void SetupAttribs() = 0;

	/**
	 * @brief Draw the object with the attached shader
	 */
	virtual void Draw() = 0;

	/**
	 * @brief Disable vertex attributes and clean up bindings
	 */
	virtual void ResetAttrib() = 0;

protected: /// gl warpping functions
	void CreateBuffer(GLenum target, GLuint *id, GLsizeiptr size, const GLvoid *data, GLenum usage);

	/**
	 * @brief call glVertexAttribPointer with VertexAttrib data.
	 * 			It is only acceptable for GL_ARRAY_BUFFER
	 *
	 * @param[in] buffer Target of array buffer object
	 * @param[in] attbs VertexAttrib data to set
	 */
	void SetupVertexAttribs(const GLuint &buffer, const std::vector<VertexAttrib> &attbs);

};

inline BasicObject *BasicObject::SetPosition(const glm::vec3 &pos) {
	mPosition = pos;
	return this;
}

inline glm::quat BasicObject::GetRotation() {
	return mOrientation;
}

inline void BasicObject::SetRotation(const glm::quat &q) {
	mOrientation = q;
}

inline void BasicObject::SetFocus(const bool &focus) {
	mFocus = focus;
}

inline void BasicObject::Select(const bool &isSelected) {
	mIsSelected = isSelected;
}

inline void BasicObject::SetVisible(const bool &isVisible) {
	mIsVisible = isVisible;
}



#endif //__BASIC_OBJECT_H__
