#include "sample/view/mrt_view.h"

#include "basic/obj/basic_camera.h"

using namespace std;

MrtView::MrtView(void *data) : SampleView(data) {}

void MrtView::OnInit() {
	string vs = File_Loader.ReadTxtFile("shader/view_tex/tex.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_tex/tex.fs");
	string tree = File_Loader.ReadTxtFile("obj3d/tree");

	TexProp tex_tree(TEX_2D_FILE, "tex/tree.bmp");

	BasicMap<PhongObj_U_Elem> po_uniforms;
	po_uniforms.mList[U_MAT_WORLD] = "worldMat";
	po_uniforms.mList[U_CAMERA_VIEW] = "viewMat";
	po_uniforms.mList[U_CAMERA_PROJ] = "projMat";
	po_uniforms.mList[U_CAMERA_POS] = "eyePos";

	mViewRenderer->GetNewObject(PHONG_OBJ, "tree", po_uniforms)
			->ImportObj(tree, 1.0f)
			->AttachShader(vs, fs, "shader")
			->AttachTexture(tex_tree, "s_tex0");


	mViewRenderer->OffAutoRotate();

	mViewRenderer->GetCamera()->SetEye(glm::vec3(50, 50, 50));
}

