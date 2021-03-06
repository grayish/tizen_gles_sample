#include "sample/view/normal_mapping_view.h"

using namespace std;

NormalMappingView::NormalMappingView(void *data) : SampleView(data) {}

void NormalMappingView::OnInit() {
	string vs = File_Loader.ReadFileToString("shader/view_nor/nor.vs");
	string fs = File_Loader.ReadFileToString("shader/view_nor/nor.fs");

	TexProp brick_color(TEX_2D_FILE, "tex/tex_c_brick.bmp");
	TexProp brick_normal(TEX_2D_FILE, "tex/tex_n_brick.bmp");

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
			->ImportObj("obj3d/teapot", 1.0f)
			->AttachShader(vs, fs, "shader")
			->AttachTexture(brick_color, "s_tex0")
			->AttachTexture(brick_normal, "s_texNor")
			->AttachLight(POINT_LT, "point_light_1", lt_uniforms);

	mViewRenderer->SetBackgroundColor(glm::vec3(0.1f));
}

