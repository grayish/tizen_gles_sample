#include "basic/tex/tex_prop.h"

#include "basic/basic_utils.h"
#include "basic/basic_file_loader.h"

using namespace std;

TexProp::TexProp(TexContainer_Type type) :
		mName(),
		mType(type),
		mWidth(0),
		mHeight(0),
		mDepth(0),
		mFormat(0),
		mInternalFormat(0),
		mDataType(0),
		mCustomParam() {
}

TexProp::TexProp(TexContainer_Type type, const string &filename, bool defaultFilter) :
		mName(filename),
		mType(type),
		mWidth(0),
		mHeight(0),
		mDepth(0),
		mFormat(0),
		mInternalFormat(0),
		mDataType(0),
		mCustomParam() {
	SetFile(filename);
	if(defaultFilter)
		SetFilter();

}

TexProp::~TexProp() {
	LOGI("destruct w:h[%d,%d]f[%d]", mWidth, mHeight, mFormat);
}

void TexProp::SetData(const std::string &name, GLsizei w, GLsizei h, GLenum fm, GLint in_fm, GLenum data_type) {
	LOGI("w:h[%d,%d]fm[%d]", w, h, fm);
	mName = name;
	mWidth = w;
	mHeight = h;
	mFormat = fm;
	mInternalFormat = in_fm;
	mDataType = data_type;
}

void TexProp::SetDataDepth(const std::string &name, GLsizei w, GLsizei h, GLsizei d, GLenum fm, int in_fm,
						   GLenum data_type) {
	LOGI("w:h[%d,%d]fm[%d]", w, h, fm);
	mName = name;
	mWidth = w;
	mHeight = h;
	mFormat = fm;
	mDepth = d;
	mInternalFormat = in_fm;
	mDataType = data_type;
}

void TexProp::SetParam(GLenum texParam, GLint val) {
	mCustomParam[texParam] = val;
}

void TexProp::SetFilter(GLint min_f, GLint mag_f, GLint warp_s, GLint warp_t) {
	SetParam(GL_TEXTURE_MIN_FILTER, min_f);
	SetParam(GL_TEXTURE_MAG_FILTER, mag_f);
	SetParam(GL_TEXTURE_WRAP_S, warp_s);
	SetParam(GL_TEXTURE_WRAP_T, warp_t);

}

void TexProp::SetFile(const std::string &filename) {
	mFiles.push_back(filename);
}

void TexProp::SetPointer(void *ptr) {
	mPtrs.push_back(ptr);
}


