#include "basic/obj/basic_object.h"

#include "basic/mgr/basic_shader_mgr.h"
#include "basic/mgr/basic_texture_mgr.h"
#include "basic/mgr/basic_light_mgr.h"
#include "basic/basic_gl_set.h"
#include "basic/basic_utils.h"

using namespace std;
using namespace glm;

BasicObject::BasicObject(const std::string &name) :
		mName(name),
		mPosition(0),
		mOrientation(),
		mFocus(false),
		mIsSelected(true),
		mIsVisible(true) {
	mOrientation = quat_cast(mat3(1));
}

BasicObject::~BasicObject() {
	LOGI("destruct name[%s]", mName.c_str());
}

BasicObject *
BasicObject::AttachShader(const std::string &vertexSource, const std::string &fragmentSource, const std::string &name) {
	Shader_Mgr.SetShaderObject(Shader_Mgr.GetNewShader(vertexSource, fragmentSource, name), this);
	return this;
}

BasicObject *BasicObject::AttachShader(const std::string &name) {
	Shader_Mgr.SetShaderObject(Shader_Mgr.GetShader(name), this);
	return this;
}

BasicObject *BasicObject::AttachLight(BasicLight_Type type, const std::string &name, const ABasicMap &map) {
	Light_Mgr.SetObjectLight(this, Light_Mgr.GetNewLight(type, name, map));
	return this;
}

BasicObject *BasicObject::AttachLight(const std::string &name) {
	Light_Mgr.SetObjectLight(this, Light_Mgr.GetLight(name));
	return this;
}

BasicObject *
BasicObject::AttachTexture(const TexProp &tex, const std::string &uniform_name) {
	Texture_Mgr.SetObjectTexture(this, tex, uniform_name);
	return this;
}

void BasicObject::CreateBuffer(GLenum target, GLuint *id, GLsizeiptr size, const GLvoid *data, GLenum usage) {
	glGenBuffers(1, id);
	check_gl_error("glGenBuffers");

	glBindBuffer(target, *id);
	check_gl_error("glBindBuffer");

	glBufferData(target, size, data, usage);
	check_gl_error("glBufferData");

	glBindBuffer(target, 0);
	check_gl_error("glBindBuffer");
}

void BasicObject::SetupVertexAttribs(const GLuint &buffer, const vector<VertexAttrib> &attbs) {
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	std::vector<VertexAttrib>::const_iterator it = attbs.begin();
	for (; it != attbs.end(); it++) {
		glVertexAttribPointer(it->index, it->size, it->type, GL_FALSE,
							  it->stride, it->pointer);
		glEnableVertexAttribArray(it->index);
	}
}

