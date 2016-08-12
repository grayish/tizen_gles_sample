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

	/**
	 * @brief Set rotation of a object
	 *
	 * @param[in] quat a quaternion to be set
	 */
	void SetRotation(const glm::quat &q);

	/**
	 * @brief Get rotation of a object
	 *
	 * @return a quaternion of the object
	 */
	glm::quat GetRotation();

	/**
	 * @brief Determine whether a object is focused or not
	 *
	 * @param[in] focus a determiner of focusing
	 */
	void SetFocus(const bool &focus);

	/**
	 * @brief Determine whether a object is selected or not
	 *
	 * @param[in] focus a determiner of selection
	 */
	void Select(const bool &isSelected);

	/**
	 * @brief Determine whether a object is visible or not
	 *
	 * @param[in] focus a determiner of visualization
	 */
	void SetVisible(const bool &isVisible);

public:
	/**
	 * @brief Constructor for the class BasicObject
	 */
	BasicObject(const std::string &name);

	/**
	 * @brief Destructor for the class BasicObject
	 */
	virtual ~BasicObject();

	/**
	 * @brief Compile a shader and attach it to a object
	 *
	 * @param[in] vertexSource The source code of the vertex shader
	 * @param[in] fragmentSource The source code of the fragment shader
	 *
	 * @return this object
	 */
	BasicObject *
	AttachShader(const std::string &vertexSource, const std::string &fragmentSource, const std::string &name);

	/**
	 * @brief Attach a pre-compiled shader to a object
	 *
	 * @param[in] name a name of the shader
	 *
	 * @return this object
	 */
	BasicObject *AttachShader(const std::string &name);

	/**
	 * @brief Create a light and attach it to a object
	 *
	 * @param[in] type a type of the light
	 * @param[in] name a name of the light
	 * @param[in] map a map of uniforms for the light
	 *
	 * @return this object
	 */
	BasicObject *AttachLight(BasicLight_Type type, const std::string &name, const ABasicMap &map);

	/**
	 * @brief Attach a light to a object
	 *
	 * @param[in] name a name of the light
	 *
	 * @return this object
	 */
	BasicObject *AttachLight(const std::string &name);

	/**
	 * @brief Create a texture of a object
	 *
	 * @param[in] tex properties of the texture
	 * @param[in] uniform_name a name of the texture in shader codes
	 *
	 * @return this object
	 */
	BasicObject *AttachTexture(const TexProp &tex, const std::string &uniform_name);

	/**
	 * @brief Set uniform in basic shader
	 */
	virtual void SetShaderUniforms(BasicShader *sh) = 0;

	/// pure virtual functions
	/**
 	 * @brief Import an obj file
 	 *
	 * @param[in] objFilename a name of the obj file
	 * @param[in] scale a scale factor
 	 */
	virtual BasicObject *ImportObj(const std::string &objFilename, const float &scale) =0 ;

protected:
	/**
	 * @brief Create buffer objects and bind buffer data
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
	 * @brief Call glVertexAttribPointer with VertexAttrib data.
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
