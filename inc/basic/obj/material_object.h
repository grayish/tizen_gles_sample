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
	std::vector<GLuint> mIndices;

	// gl buffer obj and VAO
	GLuint mBufferVertices;
	GLuint mBufferIndices;
	GLuint mVertexArrayObject;
	MaterialObj_U_Str mUniformList;
	BasicCamera *mCamera;

public: //Getter Setter
	/**
 	 * @brief Scale an object
 	 *
	 * @param[in] scale a scale factor
 	 */
	MaterialObject *SetScale(const float &scale);

public:
	/**
	 * @brief Constructor for the class MaterialObject
	 *
	 * @param[in] name a name of the object
	 * @param[in] list a map of uniforms for the object
	 * @param[in] camera a camera to be used for the object
	 */
	MaterialObject(const std::string &name, const ABasicMap *list, BasicCamera *camera);

	/**
	 * @brief Destructor for the class MaterialObject
	 */
	virtual ~MaterialObject();

	/**
 	 * @brief Import an obj file
 	 *
	 * @param[in] objFilename a name of the obj file
	 * @param[in] scale a scale factor
 	 */
	virtual BasicObject *ImportObj(const std::string &objFilename, const float &scale);

private:
	/**
 	 * @brief Import a mtl file
 	 *
	 * @param[in] mtlFilename a name of the mtl file
	 * @param[in] mtlItems a map of material data
 	 */
	void ImporterMtl(const std::string &mtlFilename, std::map<std::string, Material*> &mtlItems);

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
};

inline MaterialObject *MaterialObject::SetScale(const float &scale) {
	mScale = scale;
	return this;
}

#endif //__BASIC_MATERIAL_OBJECT_H__
