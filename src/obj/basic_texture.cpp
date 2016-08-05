#include "basic/tex/basic_texture.h"

#include "basic/basic_utils.h"
#include "basic/tex/tex_container.h"
#include "basic/basic_gl_set.h"

BasicTexture::BasicTexture(const TexContainer &tex,
						   const std::string &uniform_name,
						   const GLenum &target,
						   GLint min_filter, GLint max_filter, GLint warp_s, GLint warp_t) :
		mName(tex.filename),
		mTarget(target),
		mUniformName(uniform_name),
		mMinFilter(min_filter),
		mMagFilter(max_filter),
		mWrap_S(warp_s),
		mWrap_T(warp_t),
		mTexId() {
	Create(tex);
}

BasicTexture::BasicTexture(const TexContainer *cubeTex,
						   const std::string &uniform_name,
						   const GLenum &target,
						   GLint min_filter, GLint max_filter,
						   GLint warp_s, GLint warp_t) :
		mName(cubeTex[0].filename),
		mTarget(target),
		mUniformName(uniform_name),
		mMinFilter(min_filter),
		mMagFilter(max_filter),
		mWrap_S(warp_s),
		mWrap_T(warp_t),
		mTexId() {
	CreateCube(cubeTex);
}

BasicTexture::~BasicTexture() {
	glDeleteTextures(1, &mTexId);
}

BasicTexture *BasicTexture::Create(const TexContainer &tex) {
	glGenTextures(1, &mTexId);
	check_gl_error("glGenTextures");

	glBindTexture(mTarget, mTexId);
	check_gl_error("glBindTexture");

	TexImage(mTarget, tex);

	glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, mMinFilter);
	glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, mMagFilter);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, mWrap_S);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, mWrap_T);

	TexContainer::ParamDic::const_iterator it = tex.customParam.begin();
	for (; it != tex.customParam.end(); it++) {
		glTexParameteri(mTarget, it->first, it->second);
	}

	glBindTexture(mTarget, 0);

	return this;
}

BasicTexture *BasicTexture::CreateCube(const TexContainer *tex) {
	glGenTextures(1, &mTexId);
	check_gl_error("glGenTextures");

	glBindTexture(mTarget, mTexId);
	check_gl_error("glBindTexture");

	for (int i = 0; i < 6; i++) {
		TexImage(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), tex[i]);
	}

	glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, mMinFilter);
	glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, mMagFilter);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, mWrap_S);
	glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, mWrap_T);

	TexContainer::ParamDic::const_iterator it = tex[0].customParam.begin();
	for (; it != tex[0].customParam.end(); it++) {
		glTexParameteri(mTarget, it->first, it->second);
	}

	glBindTexture(mTarget, 0);

	return this;

}

void BasicTexture::TexImage(const GLenum &target, const TexContainer &newTex) {
	glTexImage2D(target, 0, newTex.internalFormat,
				 newTex.width, newTex.height, 0,
				 (GLenum) newTex.format, newTex.dataType,
				 (void *) newTex.pixels);
	check_gl_error("glTexImage2D");
}


