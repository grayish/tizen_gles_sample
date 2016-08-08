#ifndef __BASIC_TEXTURE_H__
#define __BASIC_TEXTURE_H__

#include <string>

#include "basic/basic_type.h"
#include "basic/basic_gl_set.h"

class TexProp;

class BasicTexture {
public:
	std::string mName;
	GLenum mTarget;
	std::string mUniformName;
	GLint mMinFilter;
	GLint mMagFilter;
	GLint mWrap_S;
	GLint mWrap_T;
	GLuint mTexId;

	BasicTexture(const TexProp &tex,
				 const std::string &uniform_name,
				 GLint min_filter = GL_LINEAR,
				 GLint mag_filter = GL_LINEAR,
				 GLint warp_s = GL_REPEAT,
				 GLint warp_t = GL_REPEAT);

	virtual ~BasicTexture();

	void Init(const TexProp &tex);
	virtual BasicTexture *Create(const TexProp &tex);

	void TexImage(const GLenum &target, const TexProp &tex);

	void TexImage2D(const GLenum &target, const TexProp &newTex, void *ptr);

	void TexImage3D(const GLenum &target, const TexProp &newTex, void *ptr);
};

#endif //__BASIC_TEXTURE_H__
