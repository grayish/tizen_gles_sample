#ifndef __BASIC_FILE_LOADER_H__
#define __BASIC_FILE_LOADER_H__

#define File_Loader FileLoader::Inst()

#include <string>
#include <fstream>

#include "basic_singleton.hpp"

class TexProp;

class FileStream {
private:
	std::string mName;
	std::ifstream mInFile;

public:
	/**
	 * @brief Constructor for the class FileStream
	 *
	 * @param[in] filename a file name to open
	 */
	FileStream(const std::string &filename);

	/**
	 * @brief Destructor for the class FileStream
	 */
	~FileStream();

	/**
	 * @brief Get a line of string from a file
	 *
	 * @param[in] buf a buffer to be written as the line
	 * @param[in] size the size of the buffer
	 */
	bool GetLine(char *buf, int size);
};

class FileLoader : public BasicSingleton<FileLoader> {
public:

	/**
	 * @brief Constructor for the class FileLoader
	 */
	FileLoader() {}

	/**
	 * @brief Destructor for the class FileLoader
	 */
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
	 * @brief Get TexData from the image file
	 *
	 * @param[in] filename Path for the source file
	 * @param[out] out Output of TexData
	 */
	void ReadTexArray(GLenum target, const TexProp& tex) const;

	/**
	 * @brief Get Content string from the text file
	 *
	 * @param[in] filename Path for the source file
	 * @param[out] out Output of string
	 */
	std::string ReadFileToString(const std::string &filename) const;
};


#endif //__BASIC_FILE_LOADER_H__


