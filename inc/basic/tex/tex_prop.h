#ifndef __BASIC_TEXDATA_H__
#define __BASIC_TEXDATA_H__

#include <string>
#include <map>
#include <vector>

#include "basic/basic_type.h"

class BasicTexture;
class BasicTextureMgr;

class TexProp {
	friend class BasicTexture;
	friend class BasicTextureMgr;
public:
	typedef std::vector<std::string> FileList;
	typedef std::vector<void *> PtrList;
	typedef std::map<GLenum, GLint> ParamDic;

private:
	std::string mName;
	TexContainer_Type mType;
	FileList mFiles;
	PtrList mPtrs;
	GLuint mWidth;
	GLuint mHeight;
	GLuint mDepth;
	GLint mFormat;
	GLint mInternalFormat;
	GLenum mDataType;
	ParamDic mCustomParam;

public:
	/**
	 * @brief Constructor for the class TexData
	 */
	TexProp(TexContainer_Type type);
	TexProp(TexContainer_Type type, const std::string &filename, bool defaultFilter = true);

	/**
	 * @brief Destructor for the class TexData
	 */
	virtual ~TexProp();

	/**
	 * @brief Set texture data
	 *
	 * @param[in] w Width of the texture
	 * @param[in] h Height of the texture
	 * @param[in] fm Format of the texture
	 */
	void SetData(const std::string &name,
				 unsigned int w = 0, unsigned int h = 0,
				 int fm = GL_RGBA, int in_fm = GL_RGBA,
				 GLenum data_type = GL_UNSIGNED_BYTE);

	void
	SetDataDepth(const std::string &name,
				 unsigned int w, unsigned int h, unsigned int d,
				 int fm, int in_fm, GLenum data_type);


	void SetFile(const std::string &filename);
	void SetPointer(void *ptr);

	void SetParam(GLenum texParam, GLint val);
	void SetFilter(GLint min_f = GL_LINEAR,
					GLint mag_f = GL_LINEAR,
					GLint warp_s = GL_REPEAT,
					GLint warp_t = GL_REPEAT);

};


#endif //__BASIC_TEXDATA_H__


