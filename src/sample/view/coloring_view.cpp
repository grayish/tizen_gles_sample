#include "sample/view/coloring_view.h"

using namespace std;

ColoringView::ColoringView(void *data) : SampleView(data) {}

void ColoringView::OnInit() {
	string vs = File_Loader.ReadTxtFile("shader/view_color/color.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_color/color.fs");
	string teapot = File_Loader.ReadTxtFile("obj3d/teapot");

	BasicMap<PhongObj_U_Elem> po_uniforms;
	po_uniforms.mList[U_MAT_WORLD] = "worldMat";
	po_uniforms.mList[U_CAMERA_VIEW] = "viewMat";
	po_uniforms.mList[U_CAMERA_PROJ] = "projMat";
	po_uniforms.mList[U_CAMERA_POS] = "eyePos";

	mViewRenderer->GetNewObject(PHONG_OBJ, "teapot", po_uniforms)
			->ImportObj(teapot, 1.0f)
			->AttachShader(vs, fs, "shader");

	mViewRenderer->SetCurrShader("shader");
}
