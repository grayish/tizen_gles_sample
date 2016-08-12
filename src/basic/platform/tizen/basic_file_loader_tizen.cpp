#ifdef BASIC_TIZEN

#include <app.h>
#include <Elementary.h>

#include "basic/basic_file_loader.h"
#include "basic/basic_utils.h"
#include "sample/sample_launcher.h"

void FileLoader::ReadTexImage2D(GLenum target, const char *filename) const {
	LOGI("file name[%s]", filename);
	Evas_Object *parent = SampleLauncher::GetInstance()->GetParent();

	int w, h;
	int surface_w, surface_h;
	evas_object_geometry_get(parent, NULL, NULL, &surface_w, &surface_h);

	Evas_Object *inline_buffer = elm_win_add(parent, "Img Read", ELM_WIN_INLINED_IMAGE);
	evas_object_move(inline_buffer, 0, 0);
	evas_object_resize(inline_buffer, surface_w, surface_h);
	evas_object_show(inline_buffer);

	Evas *canvas = evas_object_evas_get(inline_buffer);
	Evas_Object *image = evas_object_image_add(canvas);

	char path[200];
	sprintf(path, "%s%s", app_get_resource_path(), filename);
	evas_object_image_file_set(image, path, NULL);
	evas_object_image_size_get(image, &w, &h);
	evas_object_image_fill_set(image, 0, 0, w, h);
	evas_object_image_filled_set(image, EINA_TRUE);
	evas_object_resize(image, w, h);
	evas_object_show(image);

	elm_win_render(inline_buffer);

	GLubyte *pixels = static_cast<GLubyte *>(evas_object_image_data_get(image, EINA_FALSE));

	glTexImage2D(target, 0, GL_BGRA_EXT,
				 w, h, 0,
				 GL_BGRA_EXT, GL_UNSIGNED_BYTE,
				 (void *) pixels);
	check_gl_error("glTexImage2D");

	// Tizen internal format error
	/*glTexImage2D(target, 0, GL_BGRA8_EXT,
				 w, h, 0,
				 (GLenum) GL_BGRA_EXT, GL_UNSIGNED_BYTE,
				 (void *) pixels);
	check_gl_error("glTexImage2D");*/

	evas_object_del(image);
	evas_object_del(inline_buffer);

}

void FileLoader::ReadTexArray(GLenum target, const TexProp &tex) const {

	// it works
	/*GLubyte *texels = (GLubyte *) malloc (4 * 2 * 2 * 2 * sizeof(GLubyte));
	memset(texels, 255, 4 * 2 * 2 * 2 * sizeof(GLubyte));
	glTexImage3D(target, 0, GL_RGBA8, 2, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
	free(texels);
	check_gl_error("glTexImage3D");*/

	Evas_Object *parent = SampleLauncher::GetInstance()->GetParent();

	int w(0), h(0);
	int layers = tex.mFiles.size();
	int surface_w, surface_h;
	evas_object_geometry_get(parent, NULL, NULL, &surface_w, &surface_h);

	Evas_Object *inline_buffer = elm_win_add(parent, "Img Read", ELM_WIN_INLINED_IMAGE);
	evas_object_move(inline_buffer, 0, 0);
	evas_object_resize(inline_buffer, surface_w, surface_h);
	evas_object_show(inline_buffer);

	Evas *canvas = evas_object_evas_get(inline_buffer);

	for(int idx = 0; idx < layers; idx++) {
		Evas_Object *image = evas_object_image_add(canvas);
		char path[200];
		LOGI("file name[%s]", tex.mFiles[idx].c_str());
		sprintf(path, "%s%s", app_get_resource_path(), tex.mFiles[idx].c_str());
		evas_object_image_file_set(image, path, NULL);
		evas_object_image_size_get(image, &w, &h);
		evas_object_image_fill_set(image, 0, 0, w, h);
		evas_object_image_filled_set(image, EINA_TRUE);
		evas_object_resize(image, w, h);
		evas_object_show(image);

		elm_win_render(inline_buffer);

/*		int size = w * h;
		if(idx == 0) {
			array_pixels = new GLubyte[4 * size * layers];
			LOGI("arrra create size[%d]", 4 * size * layers );
		}*/

/*		GLubyte *pixels = static_cast<GLubyte *>(evas_object_image_data_get(image, EINA_FALSE));
		memcpy(array_pixels, pixels,  4 * size * sizeof(GLubyte));*/

		evas_object_del(image);
	}

//	glTexImage3D(target, 0, GL_RGBA8, w, h, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, array_pixels);
//	check_gl_error("glTexImage3D");

	evas_object_del(inline_buffer);

	// it doesn't work
	/*GLubyte *texels = (GLubyte *) malloc (4 * 2 * 2 * 2 * sizeof(GLubyte));
	memset(texels, 255, 4 * 2 * 2 * 2 * sizeof(GLubyte));
	glTexImage3D(target, 0, GL_RGBA8, 2, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
	free(texels);
	check_gl_error("glTexImage3D");*/


}

std::string FileLoader::ReadFileToString(const std::string &filename) const {
	std::string path = app_get_resource_path() + filename;
	LOGI("path %s", path.c_str());

	std::ifstream in(path, std::ios::in | std::ios::binary);

	std::string msg = "Failed to open file " + path;
	ASSERT(in.is_open() && in.good(), msg.c_str());

	std::string ret;
	in.seekg(0, std::ios::end);         // Set get position to end
	ret.resize(in.tellg());            // Resize string to support enough bytes
	in.seekg(0, std::ios::beg);         // Set get position to beginning
	in.read(&ret.at(0), ret.size());     // Read file to string

	//test
	in.seekg(0, std::ios::beg);
	char buffer[512];
	while (!in.eof()) {
		in.getline(buffer, 512);
		LOGI("getline : %s", buffer);
	}

	in.close();

	return ret;

}


FileStream::FileStream(const std::string &filename) :
		mName(filename),
		mInFile() {
	std::string path = app_get_resource_path() + filename;
	mInFile.open(path, std::ios::in | std::ios::binary);

	std::string msg = "Failed to open file " + filename;
	ASSERT(mInFile.is_open() && mInFile.good(), msg.c_str());

	mInFile.seekg(0, std::ios::beg);

}

FileStream::~FileStream() {
	if (mInFile.is_open()) {
		mInFile.close();
	}
}

bool FileStream::GetLine(char *buf, int size) {
	do { //skip empty line
		if (mInFile.eof()) {
			LOGI("end of file[%s]", mName.c_str());
			return false;
		}
		mInFile.getline(buf, size);
	} while (buf[0] == '\0');
//	LOGI("getline %s",buf);
	return true;
}

#endif

