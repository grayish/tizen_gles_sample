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
	std::vector<GLushort> mIndices;

	// gl buffer obj and VAO
	GLuint mBufferVertices;
	GLuint mBufferIndices;
	GLuint mVertexArrayObject;
	PhongObj_U_Str mUniformList;
	BasicCamera *mCamera;

public: //Getter Setter
	/**
 	 * @brief Scale an object
 	 *
	 * @param[in] scale a scale factor
 	 */
	PhongObject *SetScale(const float &scale);

	/**
 	 * @brief color an object
 	 *
	 * @param[in] col a color to be set
 	 */
	PhongObject *SetColor(const glm::vec3 &col);

public:
	/**
	 * @brief Constructor for the class PhongObject
	 *
	 * @param[in] name a name of the object
	 * @param[in] list a map of uniforms for the object
	 * @param[in] camera a camera to be used for the object
	 */
	PhongObject(const std::string &name, const ABasicMap *list, BasicCamera *camera);

	/**
	 * @brief Destructor for the class PhongObject
	 */
	virtual ~PhongObject();

	/**
 	 * @brief Import an obj file
 	 *
	 * @param[in] objFilename a name of the obj file
	 * @param[in] scale a scale factor
 	 */
	virtual BasicObject *ImportObj(const std::string &objFilename, const float &scale);

protected: // override functions
	/**
	 * @brief Create buffer objects and bind buffer data
	 */
	virtual void CreateVbo();

	/**
	 * @brief Enable vertex attribute pointers and active textures for drawing, binding data for each vertex
	 */
	virtual void SetupAttribs();

	/**
	 * @brief Disable vertex attributes and clean up bindings
	 */
	virtual void ResetAttrib();

	/**
	 * @brief Draw the object with the attached shader
	 */
	virtual void Draw();

	/**
	 * @brief Set uniforms for a shader
	 *
	 * @param[in] sh a pointer of the shader
	 */
	virtual void SetShaderUniforms(BasicShader *sh);

	/**
	 * @brief Compute tangent vectors for TBN matrix
	 */
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
