#include "basic/basic_shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "basic/basic_utils.h"
#include "basic/basic_gl_set.h"

using namespace std;
using namespace glm;

BasicShader::BasicShader(const std::string &name) :
		mName(name),
		mProgram(0),
		mUDataDictionary() {}

BasicShader::~BasicShader() {
	LOGI("destruct name[%s]", mName.c_str());
	if (mProgram > 0) {
		glDeleteProgram(mProgram);
		check_gl_error("glDeleteProgram");
	}

	typename std::map<std::string, AUniformContainer *>::iterator iter = mUDataDictionary.begin();
	for (; iter != mUDataDictionary.end(); iter++) {
		delete iter->second;
	}
}

/*** public functions ***/
void BasicShader::Use() {
	glUseProgram(mProgram);
	check_gl_error("glUseProgram");
}

void BasicShader::UseAndPassUniforms() {
	glUseProgram(mProgram);
	check_gl_error("glUseProgram");

	typename std::map<std::string, AUniformContainer *>::iterator iter = mUDataDictionary.begin();
	for (; iter != mUDataDictionary.end(); iter++) {
		iter->second->Set(this);
	}
}


GLuint BasicShader::CreateProgram(const std::string &vertexSource, const std::string &fragmentSource) {
	return CreateProgram(vertexSource.c_str(), fragmentSource.c_str());
}

GLuint BasicShader::CreateProgram(const char *vertexSource, const char *fragmentSource) {
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentSource);

	if (!vertexShader || !fragmentShader) {
		LOGE("Fail to create program!");
		return 0;
	}

	mProgram = LinkShaders(2, vertexShader, fragmentShader);
	return mProgram;
}

void BasicShader::SetUniform(const int &loc, const bool &val) const {
	glUniform1i(loc, val);
	check_gl_error("glUniform1i");
}

void BasicShader::SetUniform(const int &loc, const int &val) const {
	glUniform1i(loc, val);
	check_gl_error("glUniform1i");
}

void BasicShader::SetUniform(const int &loc, const GLuint &val) const {
	glUniform1i(loc, val);
	check_gl_error("glUniform1i");
}

void BasicShader::SetUniform(const int &loc, const float &val) const {
	glUniform1f(loc, val);
	check_gl_error("glUniform1f");
}

void BasicShader::SetUniform(const int &loc, const float &x, const float &y) const {
	glUniform2f(loc, x, y);
	check_gl_error("glUniform2f");
}

void BasicShader::SetUniform(const int &loc, const float &x, const float &y, const float &z) const {
	glUniform3f(loc, x, y, z);
	check_gl_error("glUniform3f");
}

void BasicShader::SetUniform(const int &loc, const float &x, const float &y, const float &z, const float &w) const {
	glUniform4f(loc, x, y, z, w);
	check_gl_error("glUniform4f");
}

void BasicShader::SetUniform(const int &loc, const glm::vec2 &v) const {
	glUniform2f(loc, v.x, v.y);
	check_gl_error("glUniform2f");
}

void BasicShader::SetUniform(const int &loc, const glm::vec3 &v) const {
	glUniform3f(loc, v.x, v.y, v.z);
	check_gl_error("glUniform3f");
}

void BasicShader::SetUniform(const int &loc, const glm::vec4 &v) const {
	glUniform4f(loc, v.x, v.y, v.z, v.w);
	check_gl_error("glUniform4f");
}

void BasicShader::SetUniform(const int &loc, const glm::mat3 &m) const {
	glUniformMatrix3fv(loc, 1, GL_FALSE, value_ptr(m));
	check_gl_error("glUniformMatrix3fv");
}

void BasicShader::SetUniform(const int &loc, const glm::mat4 &m) const {
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(m));
	check_gl_error("glUniformMatrix4fv");
}

GLint BasicShader::GetUniformLocation(const std::string &u_str) {
	return glGetUniformLocation(mProgram, u_str.c_str());
}

/*** private functions ***/
GLuint BasicShader::LoadShader(GLenum shaderType, const char *source) const {
	GLuint shader = glCreateShader(shaderType);

	if (shader) {
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen) {
				char *buf = static_cast<char *>(malloc((size_t) infoLen));

				if (buf) {
					glGetShaderInfoLog(shader, infoLen, nullptr, buf);
					LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
					free(buf);
				}

				glDeleteShader(shader);
			}
			exit(1);
		}
	}
	return shader;
}

GLuint BasicShader::LinkShaders(int nArgs, ...) const {
	GLuint program = glCreateProgram();
	check_gl_error("glCreateProgram");
	if (program) {
		va_list ap;
		va_start(ap, nArgs);

		for (int i = 1; i <= nArgs; ++i) {
			glAttachShader(program, va_arg(ap, GLint));
			check_gl_error("glAttachShader");
		}

		/* FOR GLES 20
		glBindAttribLocation(program, V_ATTRIB_POSITION, "position");
		glBindAttribLocation(program, V_ATTRIB_NORMAL, "normal");
		glBindAttribLocation(program, V_ATTRIB_TEX, "texCoord");
		glBindAttribLocation(program, V_ATTRIB_TANGENT, "tangent");
		check_gl_error("glBindAttribLocation");
		 */

		glLinkProgram(program);
		check_gl_error("glLinkProgram");

		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char *buf = static_cast<char *>(malloc((size_t) bufLength));
				if (buf) {
					glGetProgramInfoLog(program, bufLength, nullptr, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			exit(1);
		}
	}

	return program;
}

