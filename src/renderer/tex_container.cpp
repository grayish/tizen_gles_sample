#include "basic/tex/tex_container.h"

#include "basic/basic_utils.h"
#include "basic/basic_file_loader.h"

using namespace std;

TexContainer::TexContainer() :
		pixels(nullptr),
		width(0),
		height(0),
		format(0),
		internalFormat(0),
		dataType(0),
		customParam() {
}

TexContainer::~TexContainer() {
	LOGI("w:h[%d,%d]f[%d]\n", width, height, format);
	if (pixels) delete pixels;
}

void TexContainer::SetData(int size, const Bubyte *source,
						   unsigned int w, unsigned int h, int fm) {
	LOGI("w:h[%d,%d]fm[%d]\n", w, h, fm);
	//if(pixels) delete pixels;
	int channels(4);
	//if (fm == GL_RGBA || fm == GL_BGRA )	 { channels = 4; }
	//if (fm == GL_RGB  || fm == GL_BGR  )	 { channels = 3; }
	pixels = new Bubyte[size * channels];
	memcpy(this->pixels, source, channels * size * sizeof(Bubyte));
	width = w;
	height = h;
	format = fm;
	internalFormat = fm;
	dataType = BGL_UNSIGNED_BYTE;
}

void TexContainer::SetParam(Benum texParam, Bint val) {
	customParam[texParam] = val;
}
