#include "sample/view/instance_view.h"

#include "basic/obj/instanced_object.h"

using namespace std;

InstanceView::InstanceView(void *data) : SampleView(data) {}

void InstanceView::OnInit() {
	string vs = File_Loader.ReadTxtFile("shader/view_inst/inst.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_inst/inst.fs");

	string cube = File_Loader.ReadTxtFile("obj3d/sphere");

	BasicMap<InstObj_U_Elem> io_u;
	io_u.mList[U_INST_MAT_WORLD] = "worldMat";
	io_u.mList[U_INST_CAMERA_VIEW] = "viewMat";
	io_u.mList[U_INST_CAMERA_PROJ] = "projMat";

	BasicObject *obj =
			mViewRenderer->GetNewObject(INST_OBJ, "cube", io_u)
					->ImportObj(cube, 1.0f)
					->AttachShader(vs, fs, "inst_shader");

	dynamic_cast<InstObject *>(obj)
			->SetInstancing(15 * 15, 15, 15, 5.0f);
}



