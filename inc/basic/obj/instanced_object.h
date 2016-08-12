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
	/**
	 * @brief Constructor for the class InstObject
	 *
	 * @param[in] name a name of the object
	 * @param[in] list a map of uniforms for the instanced object
	 * @param[in] camera a camera to be used for the object
	 */
	InstObject(const std::string &name, const ABasicMap *list, BasicCamera *camera);

	/**
	 * @brief Destructor for the class InstObject
	 */
	virtual ~InstObject();

	/**
 	 * @brief Import an obj file
 	 *
	 * @param[in] objFilename a name of the obj file
	 * @param[in] scale a scale factor
	 *
	 * @return this object
 	 */
	virtual BasicObject *ImportObj(const std::string &objFilename, const float &scale);

	/**
 	 * @brief Create a vertex attribute of transform matrix for instancing
 	 *
	 * @param[in] count a count of instances
	 * @param[in] row the number of rows
	 * @param[in] column the number of columns
	 * @param[in] gridSize a size of grid cell
 	 */
	void SetInstancing(const int &count, const int &row, const int &column, const float &gridSize);

protected:
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

#endif //__INSTANCED_OBJECT_H__
