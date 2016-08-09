#ifndef __BASIC_SHADER_H__
#define __BASIC_SHADER_H__

#include <map>
#include <vector>

#include "basic_type.h"
#include "basic_utils.h"

class BasicShaderMgr;

class BasicShader {
	friend class BasicShaderMgr;

private:
	class AUniformContainer {
	public:
		AUniformContainer() {}

		virtual ~AUniformContainer() {}

		virtual void Set(BasicShader *shader) = 0;
	};

	template<typename T>
	class UniformContainer : public AUniformContainer {
	public:
		std::string mUniformName;
		int mLocation;
		T mData;

		/**
         * @brief Constructor for the class UniformContainer
         *
         * @param[in] name name of a uniform variable
         * @param[in] loc location of the uniform variable
         * @param[in] data data to be set in the uniform variable
         */
		UniformContainer(const std::string &name,
						 int loc,
						 T data) :
				AUniformContainer(),
				mUniformName(name),
				mLocation(loc),
				mData(data) {}

		/**
         * @brief Destructor for the class UniformContainer
         */
		virtual ~UniformContainer() {}

		/**
         * @brief Set uniform variable of a shader
         *
         * @param[in] shader a pointer of the target shader
         */
		virtual void Set(BasicShader *shader) {
			if(mLocation >= 0)
				shader->SetUniform(mLocation, mData);
		}
	};

private: // glProgram obj member
	std::string mName;
	GLuint mProgram;
	std::map<std::string, AUniformContainer *> mUDataDictionary;

public: // inline getter and setter
	const GLuint GetProgram() const;

public:

	/**
     * @brief Constructor for the class BasicShader
     */
	BasicShader(const std::string &name); // constructor
	virtual ~BasicShader(); // destructor

	// public functions
	/**
	 * @brief Use program set
	 */
	void Use();

	void UseAndPassUniforms();

	/**
	 * @brief Create shader program with shader source codes
	 *
	 * @param[in] vertexSource Source code of vertex shader
	 * @param[in] fragmentSource Source code of fragment shader
	 *
	 * @return the shader object
	 */
	GLuint CreateProgram(const std::string &vertexSource, const std::string &fragmentSource);

	/**
	 * @brief Create shader program with shader source codes
	 *
	 * @param[in] vertexSource Source code of vertex shader
	 * @param[in] fragmentSource Source code of fragment shader
	 *
	 * @return the shader object
	 */
	GLuint CreateProgram(const char *vertexSource, const char *fragmentSource);


	/**
 * @brief Set a new uniform variable
 *
 * @param[in] type a type of a uniform variable
 * @param[in] name name of a uniform variable
 * @param[in] data data to be set in the uniform variable
 */
	template<typename T>
	void SetUniformData(const std::string &name, const T &data);

	// overload functions to set uniforms : glUniform##
	/**
	 * @brief set uniform variable to bool value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] val bool value to be set
	 */
	void SetUniform(const int &loc, const bool &val) const;

	/**
	 * @brief set uniform variable to int value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] val int value to be set
	 */
	void SetUniform(const int &loc, const int &val) const;

	/**
	 * @brief set uniform variable to GLuint value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] val GLuint value to be set
	 */
	void SetUniform(const int &loc, const GLuint &val) const;

	/**
	 * @brief set uniform variable to float value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] val Float value to be set
	 */
	void SetUniform(const int &loc, const float &val) const;

	/**
	 * @brief set uniform variable to vec2 value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] x x value of the vector
	 * @param[in] y y value of the vector
	 */
	void SetUniform(const int &loc, const float &x, const float &y) const;

	/**
	 * @brief set uniform variable to vec3 value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] x x value of the vector
	 * @param[in] y y value of the vector
	 * @param[in] z z value of the vector
	 */
	void SetUniform(const int &loc, const float &x, const float &y, const float &z) const;

	/**
	 * @brief set uniform variable to vec4 value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] x x value of the vector
	 * @param[in] y y value of the vector
	 * @param[in] z z value of the vector
	 * @param[in] w w value of the vector
	 */
	void SetUniform(const int &loc, const float &x, const float &y, const float &z, const float &w) const;

	/**
	 * @brief set uniform variable to vec2 value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] v vec2 value to be set
	 */
	void SetUniform(const int &loc, const glm::vec2 &v) const;

	/**
	 * @brief set uniform variable to vec3 value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] v vec3 value to be set
	 */
	void SetUniform(const int &loc, const glm::vec3 &v) const;

	/**
	 * @brief set uniform variable to vec4 value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] v vec4 value to be set
	 */
	void SetUniform(const int &loc, const glm::vec4 &v) const;

	/**
	 * @brief set uniform variable to mat3 value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] m mat3 value to be set
	 */
	void SetUniform(const int &loc, const glm::mat3 &m) const;

	/**
	 * @brief set uniform variable to mat4 value
	 *
	 * @param[in] loc Location of the uniform variable
	 * @param[in] m mat4 value to be set
	 */
	void SetUniform(const int &loc, const glm::mat4 &m) const;

	GLint GetUniformLocation(const std::string &u_str);

private:
	/**
	 * @brief Create and compile a shader object
	 *
	 * @param[in] shaderType Type of shader
	 * @param[in] source Source code of the shader
	 *
	 * @return the shader object
	 */
	GLuint LoadShader(GLenum shaderType, const char *source) const;

	/**
	 * @brief Attach shaders to program set and link the program
	 *
	 * @param[in] nArgs The number of shader objects
	 * @param[in] va_arg Shader objects to be attached
	 *
	 * @return the program object
	 */
	GLuint LinkShaders(int nArgs, ...) const;

};

inline const GLuint BasicShader::GetProgram() const {
	return mProgram;
}

template<typename T>
void BasicShader::SetUniformData(const std::string &name, const T &data) {
	typename std::map<std::string, AUniformContainer *>::iterator iter;
	iter = mUDataDictionary.find(name);

	if (iter == mUDataDictionary.end()) {
		GLint loc = GetUniformLocation(name);
		if(loc < 0) {
			LOGE("program[%s]: uniform[%s] doesn't exist!", mName.c_str(), name.c_str());
		}
		AUniformContainer *newCon = new UniformContainer<T>( name.c_str(), loc, data);
		mUDataDictionary[name] = newCon;
	} else {
		UniformContainer<T> *con = dynamic_cast<UniformContainer<T> *>(mUDataDictionary[name]);
		con->mData = data;
	}
}


#endif //__BASIC_SHADER_H__


