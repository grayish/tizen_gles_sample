#include "sample/view/vertex_lighting_view.h"

using namespace std;

VertexLightingView::VertexLightingView(void *data) : SampleView(data) {}

void VertexLightingView::OnInit() {
	string vs = File_Loader.ReadFileToString("shader/view_v_lit/v_lit.vs");
	string fs = File_Loader.ReadFileToString("shader/view_v_lit/v_lit.fs");

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
			->AttachLight(POINT_LT, "point_light_1", lt_uniforms);

}


