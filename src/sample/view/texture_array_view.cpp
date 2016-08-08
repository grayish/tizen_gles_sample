#include "sample/view/texture_array_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

TextureArrayView::TextureArrayView(void *data) : SampleView(data) {
}

#define FileLoader FileLoader::Inst()

void TextureArrayView::OnInit() {/*
	string vs = FileLoader.ReadTxtFile("shader/view_tex_array/tex_array.vs");
	string fs = FileLoader.ReadTxtFile("shader/view_tex_array/tex_array.fs");

	string cube = FileLoader.ReadTxtFile("obj3d/cube");

	TexProp sequences[2];
	FileLoader.ReadTexture("tex/tex_c_brick.bmp", sequences[0]);
	FileLoader.ReadTexImage2D("tex/tex_n_brick.bmp", sequences[1]);

	BasicObject* obj;
	float scale = 1.0f;

	obj = mViewRenderer->SetNewModel(cube, scale);
	obj->AttachShader(vs, fs);
	obj->SetTexArray(sequences, 2);

	mViewRenderer->Initialize();

	//mViewRenderer->OffAutoRotate();
	mViewRenderer->GetCamera()->SetEye(25, 25, 25);
	mViewRenderer->GetCamera()->SetAt(0, 0, 0);
	mViewRenderer->SetFocus(obj);*/
}

