#ifndef __BASIC_FILE_LOADER_H__
#define __BASIC_FILE_LOADER_H__

#define File_Loader FileLoader::Inst()

#include <string>
#include <fstream>

#include "basic_singleton.hpp"

class FileStream {
private:
	std::string mName;
	std::ifstream mInFile;

public:
	FileStream(const std::string &filename);
	~FileStream();
	bool GetLine(char *buf, int size);
};

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
	void ReadTexImage2D(GLenum target, const char *filename) const;

	/**
	 * @brief Get Content string from the text file
	 *
	 * @param[in] filename Path for the source file
	 * @param[out] out Output of string
	 */
	std::string ReadFileToString(const std::string &filename) const;
};


#endif //__BASIC_FILE_LOADER_H__


