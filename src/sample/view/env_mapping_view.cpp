#include "sample/view/env_mapping_view.h"

using namespace std;

EnvMappingView::EnvMappingView(void *data) : SampleView(data) {}

void EnvMappingView::OnInit() {
	string vs = File_Loader.ReadFileToString("shader/view_env/env.vs");
	string fs = File_Loader.ReadFileToString("shader/view_env/env.fs");

	TexProp cubemap(TEX_CUBE_FILE);
	cubemap.SetData("cube_map");
	cubemap.SetFilter();
	cubemap.SetFile("tex/cm_left.bmp");// X+
	cubemap.SetFile("tex/cm_right.bmp");// X-
	cubemap.SetFile("tex/cm_top.bmp");// Y+
	cubemap.SetFile("tex/cm_bottom.bmp");// Y-
	cubemap.SetFile("tex/cm_back.bmp");// Z+
	cubemap.SetFile("tex/cm_front.bmp");// Z-

	BasicMap<PhongObj_U_Elem> po_uniforms;
	po_uniforms.mList[U_MAT_WORLD] = "worldMat";
	po_uniforms.mList[U_CAMERA_VIEW] = "viewMat";
	po_uniforms.mList[U_CAMERA_PROJ] = "projMat";
	po_uniforms.mList[U_CAMERA_POS] = "eyePos";

	TexProp tizen(TEX_2D_FILE, "tex/tizen.png");

	mViewRenderer->GetNewObject(PHONG_OBJ, "teapot", po_uniforms)
			->ImportObj("obj3d/teapot", 1.0f)
			->AttachShader(vs, fs, "shader")
			->AttachTexture(cubemap, "s_texCube")
			->AttachTexture(tizen, "s_tex0");

}


