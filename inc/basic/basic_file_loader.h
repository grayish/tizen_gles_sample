#ifndef __BASIC_FILE_LOADER_H__
#define __BASIC_FILE_LOADER_H__

#define File_Loader FileLoader::Inst()

#include <string>

#include "basic_singleton.hpp"

class TexContainer;

class FileLoader : public BasicSingleton<FileLoader> {
public:
	/**
	 * @brief Constructor for the class FileLoader
	 */
	FileLoader() {}

	virtual ~FileLoader() {}

	void Release() {}

	/**
	 * @brief Get TexData from the image file
	 *
	 * @param[in] filename Path for the source file
	 * @param[out] out Output of TexData
	 */
	void ReadTexture(const char *filename, TexContainer &out) const;

	/**
	 * @brief Get Content string from the text file
	 *
	 * @param[in] filename Path for the source file
	 * @param[out] out Output of string
	 */
	std::string ReadTxtFile(const std::string &filename) const;
};


#endif //__BASIC_FILE_LOADER_H__


