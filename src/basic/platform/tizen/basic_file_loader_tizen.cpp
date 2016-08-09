#ifdef BASIC_TIZEN

#include <app.h>
#include <Elementary.h>
#include <fstream>

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
				 (GLenum) GL_BGRA_EXT, GL_UNSIGNED_BYTE,
				 (void *) pixels);
	check_gl_error("glTexImage2D");

	evas_object_del(image);
	evas_object_del(inline_buffer);

}

std::string FileLoader::ReadTxtFile(const std::string &filename) const {
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
	in.close();

	return ret;

}


#endif
