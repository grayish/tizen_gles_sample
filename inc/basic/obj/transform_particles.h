#ifndef __TRANSFORM_OBJECT_H__
#define __TRANSFORM_OBJECT_H__

#define NUM_PARTICLES   300
#define SIM_STEP       0.005f

#include "basic_object.h"

#include <map>

class BasicObjectMgr;

class TransformParticles : public BasicObject {
	friend class BasicObjectMgr;

private:
	typedef std::map<TransformObj_U_Elem, std::string> TransformObj_U_Str;

	glm::vec3 mColor;

	// vertex data array and indices
	std::vector<Particle> mVertices;

	// gl buffer obj and VAO
	GLuint mBufferVertices[2];
	GLuint mSrcBuffer;
	GLuint mDstBuffer;
	TransformObj_U_Str mUniformList;
	GLsync mSync;
	float mTime;
	BasicShader* mTransformShader;

public: //Getter Setter

	/**
 	 * @brief color an object
 	 *
	 * @param[in] col a color to be set
 	 */
	TransformParticles *SetColor(const glm::vec3 &col);

public:
	/**
	 * @brief Constructor for the class TransformParticles
	 *
	 * @param[in] name a name of the object
	 * @param[in] list a map of uniforms for the object
	 */
	TransformParticles(const std::string &name, const ABasicMap *list);

	/**
	 * @brief Destructor for the class TransformParticles
	 */
	virtual ~TransformParticles();

	/**
	 * @brief Create a shader and attach it to a object in order to use transform feedback
	 *
	 * @param[in] vs a code of a vertex shader
	 * @param[in] fs a code of a fragment shader
	 * @param[in] varing names of varying variables
	 * @param[in] v_cnt the number of varying variables
	 * @param[in] name a name of the shader
	 */
	void Attatch_TF_Shader(const std::string &vs, const std::string &fs, const char **varyings, int v_cnt,
							   const std::string &name);

	/**
	 * @brief Update particles by stepping forward
	 */
	void UpdateParticles(const glm::vec2 &screen_pt);


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
 	 * @brief Import an obj file
 	 *
	 * @param[in] objFilename a name of the obj file
	 * @param[in] scale a scale factor
 	 */
	virtual BasicObject *ImportObj(const std::string &objSource, const float &scale);

private:
	/**
	 * @brief Initiate particles
	 */
	void InitParticle();

};

inline TransformParticles *TransformParticles::SetColor(const glm::vec3 &col) {
	mColor = col;
	return this;
}

#endif //__TRANSFORM_OBJECT_H__
