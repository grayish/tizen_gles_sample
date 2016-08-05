#ifndef __BASIC_TEXDATA_H__
#define __BASIC_TEXDATA_H__

#include <string>
#include <map>

#include "basic/basic_type.h"

class TexContainer {
public:
	typedef std::map<Benum, Bint> ParamDic;

	std::string filename;
	Bubyte *pixels;
	Buint width;
	Buint height;
	Bint format;
	Bint internalFormat;
	Benum dataType;
	ParamDic customParam;

	/**
	 * @brief Constructor for the class TexData
	 */
	TexContainer();

	/**
	 * @brief Destructor for the class TexData
	 */
	~TexContainer();

	/**
	 * @brief Set texture data
	 *
	 * @param[in] size The size of source data
	 * @param[in] source data of source image
	 * @param[in] w Width of the texture
	 * @param[in] h Height of the texture
	 * @param[in] fm Format of the texture
	 */
	void SetData(int size, const Bubyte *source,
				 unsigned int w, unsigned int h, int fm);

	void SetParam(Benum texParam, Bint val);


};


#endif //__BASIC_TEXDATA_H__


