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
	GLuint mTexId;

	BasicTexture(const TexProp &tex,
				 const std::string &uniform_name);

	virtual ~BasicTexture();

	void Init(const TexProp &tex);
	virtual BasicTexture *Create(const TexProp &tex);

	void TexImage(const GLenum &target, const TexProp &tex);

	void TexImage2D(const GLenum &target, const TexProp &newTex, void *ptr);

	void TexImage3D(const GLenum &target, const TexProp &newTex, void *ptr);
};

#endif //__BASIC_TEXTURE_H__
