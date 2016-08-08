#include "sample/view/frag_lighting_view.h"

using namespace std;

FragLightingView::FragLightingView(void *data) : SampleView(data) {}

void FragLightingView::OnInit() {
	string vs = File_Loader.ReadTxtFile("shader/view_f_lit/f_lit.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_f_lit/f_lit.fs");
	string teapot = File_Loader.ReadTxtFile("obj3d/teapot");

	TexProp tizenTex(TEX_2D_FILE, "tex/tizen_black.png");

	BasicMap<PhongObj_U_Elem> po_uniforms;
	po_uniforms.mList[U_MAT_WORLD] = "worldMat";
	po_uniforms.mList[U_CAMERA_VIEW] = "viewMat";
	po_uniforms.mList[U_CAMERA_PROJ] = "projMat";
	po_uniforms.mList[U_CAMERA_POS] = "eyePos";
	po_uniforms.mList[U_MAT_DIFFUSE] = "materialDiff";
	po_uniforms.mList[U_MAT_SPECULAR] = "materialSpec";
	po_uniforms.mList[U_MAT_AMBIENT] = "materialAmbi";
	po_uniforms.mList[U_MAT_EMITION] = "materialEmit";
	po_uniforms.mList[U_MAT_SHINESS] = "materialSh";

	BasicMap<PointLt_U_Elem> lt_uniforms;
	lt_uniforms.mList[U_PL_DIFFUSE] = "sourceDiff";
	lt_uniforms.mList[U_PL_AMBIENT] = "sourceAmbi";
	lt_uniforms.mList[U_PL_SPECULAR] = "sourceSpec";
	lt_uniforms.mList[U_PL_POS] = "lightPos";

	mViewRenderer->GetNewObject(PHONG_OBJ, "teapot", po_uniforms)
			->ImportObj(teapot, 1.0f)
			->AttachShader(vs, fs, "shader")
			->AttachTexture(tizenTex, "s_tex0")
			->AttachLight(POINT_LT, "point_light_1", lt_uniforms);

}

