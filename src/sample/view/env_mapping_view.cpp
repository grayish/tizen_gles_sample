#include "sample/view/env_mapping_view.h"

using namespace std;

EnvMappingView::EnvMappingView(void *data) : SampleView(data) {}

void EnvMappingView::OnInit() {
	string vs = File_Loader.ReadTxtFile("shader/view_env/env.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_env/env.fs");
	string teapot = File_Loader.ReadTxtFile("obj3d/teapot");

	TexContainer cubemap[6];
	File_Loader.ReadTexture("tex/cm_left.bmp", cubemap[0]); // X+
	File_Loader.ReadTexture("tex/cm_right.bmp", cubemap[1]); // X-
	File_Loader.ReadTexture("tex/cm_top.bmp", cubemap[2]); // Y+
	File_Loader.ReadTexture("tex/cm_bottom.bmp", cubemap[3]); // Y-
	File_Loader.ReadTexture("tex/cm_back.bmp", cubemap[4]); // Z+
	File_Loader.ReadTexture("tex/cm_front.bmp", cubemap[5]); // Z-

	BasicMap<PhongObj_U_Elem> po_uniforms;
	po_uniforms.mList[U_MAT_WORLD] = "worldMat";
	po_uniforms.mList[U_CAMERA_VIEW] = "viewMat";
	po_uniforms.mList[U_CAMERA_PROJ] = "projMat";
	po_uniforms.mList[U_CAMERA_POS] = "eyePos";

	mViewRenderer->GetNewObject(PHONG_OBJ, "teapot", po_uniforms)
			->ImportObj(teapot, 1.0f)
			->AttachShader(vs, fs, "shader")
			->AttachCubeTex(cubemap, "s_texCube");

}


