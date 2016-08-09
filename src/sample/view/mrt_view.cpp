#include "sample/view/mrt_view.h"

#include "basic/obj/basic_camera.h"

using namespace std;

MrtView::MrtView(void *data) : SampleView(data) {}

void MrtView::OnInit() {
	string vs = File_Loader.ReadTxtFile("shader/view_mrt/mrt.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_mrt/mrt.fs");
	string chess = File_Loader.ReadTxtFile("obj3d/chess");

	BasicMap<MaterialObj_U_Elem> mo_uniforms;
	mo_uniforms.mList[MTL_U_MAT_WORLD] = "worldMat";
	mo_uniforms.mList[MTL_U_CAMERA_VIEW] = "viewMat";
	mo_uniforms.mList[MTL_U_CAMERA_PROJ] = "projMat";
	mo_uniforms.mList[MTL_U_CAMERA_POS] = "eyePos";
	mo_uniforms.mList[MTL_U_MAT_SHINESS] = "materialSh";

	BasicMap<PointLt_U_Elem> lt_uniforms;
	lt_uniforms.mList[U_PL_DIFFUSE] = "sourceDiff";
	lt_uniforms.mList[U_PL_AMBIENT] = "sourceAmbi";
	lt_uniforms.mList[U_PL_SPECULAR] = "sourceSpec";
	lt_uniforms.mList[U_PL_POS] = "lightPos";

	mViewRenderer->GetNewObject(MATERIAL_OBJ, "chess", mo_uniforms)
			->ImportObj(chess, 1.0f)
			->AttachShader(vs, fs, "shader")
			->AttachLight(POINT_LT, "point_light_1", lt_uniforms);


	mViewRenderer->OffAutoRotate();

//	mViewRenderer->GetCamera()->SetEye(glm::vec3(50, 50, 50));
}

