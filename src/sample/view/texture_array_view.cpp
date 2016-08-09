#include "sample/view/texture_array_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

TextureArrayView::TextureArrayView(void *data) : SampleView(data) {
}

void TextureArrayView::OnInit() {
	string vs = File_Loader.ReadFileToString("shader/view_tex_array/tex_array.vs");
	string fs = File_Loader.ReadFileToString("shader/view_tex_array/tex_array.fs");

	BasicMap<PhongObj_U_Elem> po_uniforms;
	po_uniforms.mList[U_MAT_WORLD] = "worldMat";
	po_uniforms.mList[U_CAMERA_VIEW] = "viewMat";
	po_uniforms.mList[U_CAMERA_PROJ] = "projMat";
	po_uniforms.mList[U_CAMERA_POS] = "eyePos";

	TexProp texArray(TEX_2D_ARRAY_FILE);
	texArray.SetData("tex_array");
	texArray.SetFile("tex/tizen_black.png");
	texArray.SetFile("tex/tizen_noalpha.png");
	texArray.SetFilter();

	mViewRenderer->GetNewObject(PHONG_OBJ, "cube", po_uniforms)
			->ImportObj("obj3d/cube", 1.0f)
			->AttachShader(vs, fs, "sh_tex_array")
			->AttachTexture(texArray, "s_texArray");

	mViewRenderer->SetBackgroundColor(glm::vec3(0.1f));


}

