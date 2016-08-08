#include "sample/view/texturing_view.h"

using namespace std;

TexturingView::TexturingView(void *data) : SampleView(data) {}

void TexturingView::OnInit() {
	string vs = File_Loader.ReadTxtFile("shader/view_tex/tex.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_tex/tex.fs");
	string teapot = File_Loader.ReadTxtFile("obj3d/teapot");

	TexProp texTizen(TEX_2D_FILE, "tex/Fig7_xx_Tex2.bmp");

	BasicMap<PhongObj_U_Elem> po_uniforms;
	po_uniforms.mList[U_MAT_WORLD] = "worldMat";
	po_uniforms.mList[U_CAMERA_VIEW] = "viewMat";
	po_uniforms.mList[U_CAMERA_PROJ] = "projMat";
	po_uniforms.mList[U_CAMERA_POS] = "eyePos";

	mViewRenderer->GetNewObject(PHONG_OBJ, "teapot", po_uniforms)
			->ImportObj(teapot, 1.0f)
			->AttachShader(vs, fs, "shader")
			->AttachTexture(texTizen, "s_tex0");

}

