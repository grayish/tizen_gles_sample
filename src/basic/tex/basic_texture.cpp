#include "basic/tex/basic_texture.h"

#include "basic/basic_utils.h"
#include "basic/tex/tex_prop.h"
#include "basic/basic_file_loader.h"

BasicTexture::BasicTexture(const TexProp &tex, const std::string &uniform_name) :
		mName(tex.mName),
		mTarget(),
		mUniformName(uniform_name),
		mTexId() {
	Init(tex);
}

void BasicTexture::Init(const TexProp &tex) {
	switch (tex.mType) {
		case TEX_2D_FILE :
		case TEX_2D_PTR :
			mTarget = GL_TEXTURE_2D;
			break;
		case TEX_3D_FILE :
		case TEX_3D_PTR :
			mTarget = GL_TEXTURE_3D;
			break;
		case TEX_CUBE_FILE :
		case TEX_CUBE_PTR :
			mTarget = GL_TEXTURE_CUBE_MAP;
			break;
		case TEX_2D_ARRAY_FILE :
		case TEX_2D_ARRAY_PTR :
			mTarget = GL_TEXTURE_2D_ARRAY;
			break;
		default:
			break;
	}

	Create(tex);

}

BasicTexture::~BasicTexture() {
	LOGI("destruct name[%s]", mName.c_str());
	glDeleteTextures(1, &mTexId);
	check_gl_error("glDeleteTextures");
}

BasicTexture *BasicTexture::Create(const TexProp &tex) {
	glGenTextures(1, &mTexId);
	check_gl_error("glGenTextures");

	glBindTexture(mTarget, mTexId);
	check_gl_error("glBindTexture");

	TexImage(mTarget, tex);

	TexProp::ParamDic::const_iterator it = tex.mCustomParam.begin();
	for (; it != tex.mCustomParam.end(); it++) {
		glTexParameteri(mTarget, it->first, it->second);
	}

	glBindTexture(mTarget, 0);

	return this;
}

void BasicTexture::TexImage2D(const GLenum &target, const TexProp &newTex, void *ptr) {
	glTexImage2D(target, 0, newTex.mInternalFormat,
				 newTex.mWidth, newTex.mWidth, 0,
				 newTex.mFormat, newTex.mDataType, ptr);
	check_gl_error("glTexImage2D");
}

void BasicTexture::TexImage3D(const GLenum &target, const TexProp &newTex, void *ptr) {
	glTexImage3D(target, 0, newTex.mInternalFormat,
				 newTex.mWidth, newTex.mWidth, newTex.mDepth, 0,
				 newTex.mFormat, newTex.mDataType, ptr);
	check_gl_error("glTexImage3D");
}


void BasicTexture::TexImage(const GLenum &target, const TexProp &tex) {
	switch (tex.mType) {
		case TEX_2D_FILE :
			File_Loader.ReadTexImage2D(target, tex.mFiles[0].c_str());
			break;
		case TEX_2D_PTR :
			TexImage2D(target, tex, tex.mPtrs[0]);
			break;
		case TEX_CUBE_FILE : {
			for (unsigned int i = 0; i < 6; i++) {
				File_Loader.ReadTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tex.mFiles[i].c_str());
			}
		}
			break;
		case TEX_CUBE_PTR : {
			for (unsigned int i = 0; i < 6; i++) {
				TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tex, tex.mPtrs[i]);
			}
		}
			break;
		case TEX_3D_FILE:
			break;
		case TEX_3D_PTR:
			TexImage3D(target, tex, tex.mPtrs[0]);
			break;

		case TEX_2D_ARRAY_FILE: {
			File_Loader.ReadTexArray(mTarget, tex);
		}
			break;

		case TEX_2D_ARRAY_PTR : {
			GLubyte texels[32] =
					{
							//Texels for first image.
							0,   0,   0,   255,
							255, 0,   0,   255,
							0,   255, 0,   255,
							0,   0,   255, 255,
							//Texels for second image.
							255, 255, 255, 255,
							255, 255,   0, 255,
							0,   255, 255, 255,
							255, 0,   255, 255,
					};
//			glTexStorage3D(mTarget, 1, GL_BGRA8_EXT, 2, 2, 2); // ERROR!
			glTexStorage3D(mTarget, 1, GL_BGRA_EXT, 2, 2, 2);
			check_gl_error("glTexStorage3D");
			glTexSubImage3D(mTarget, 0, 0, 0, 0, 2, 2, 2, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texels); // ERROR!
			check_gl_error("glTexSubImage3D");
//			TexImage3D(target, tex, tex.mPtrs[0]);
		}
			break;
		default:
			break;

	}

}


