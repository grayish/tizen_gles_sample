#ifndef __BASIC_TEXTURE_H__
#define __BASIC_TEXTURE_H__

#include <string>

#include "basic/basic_type.h"

class TexContainer;

class BasicTexture {
public:
	std::string mName;
	Benum mTarget;
	std::string mUniformName;
	Bint mMinFilter;
	Bint mMagFilter;
	Bint mWrap_S;
	Bint mWrap_T;
	Buint mTexId;

	BasicTexture(const TexContainer &tex,
				 const std::string &uniform_name,
				 const Benum &target,
				 Bint min_filter = BGL_LINEAR,
				 Bint max_filter = BGL_LINEAR,
				 Bint warp_s = BGL_REPEAT,
				 Bint warp_t = BGL_REPEAT);

	BasicTexture(const TexContainer *cubeTex,
				 const std::string &uniform_name,
				 const Benum &target = BGL_TEXTURE_CUBE_MAP,
				 Bint min_filter = BGL_LINEAR,
				 Bint max_filter = BGL_LINEAR,
				 Bint warp_s = BGL_REPEAT,
				 Bint warp_t = BGL_REPEAT);

	virtual ~BasicTexture();

	virtual BasicTexture *Create(const TexContainer &tex);

	virtual BasicTexture *CreateCube(const TexContainer *tex);

	void TexImage(const Benum &target, const TexContainer &newTex);
};

#endif //__BASIC_TEXTURE_H__
