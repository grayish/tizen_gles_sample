#include "sample/view/texture_array_view.h"

#include "basic/mgr/basic_shader_mgr.h"
#include "basic/basic_shader.h"

using namespace std;

TextureArrayView::TextureArrayView(void *data) : SampleView(data) {
}

void TextureArrayView::OnInit() {
	string vs = File_Loader.ReadFileToString("shader/view_tex_array/tex_array.vs");
	string fs = File_Loader.ReadFileToString("shader/view_tex_array/tex_array.fs");

	BasicMap<PhongObj_U_Elem> po_uniforms;
	po_uniforms.mList[U_MAT_WORLD] = "worldMat";
	po_uniforms.mList[U_CAMERA_VIEW] = "viewMat";
	po_uniforms.mList[U_CAMERA_PROJ] = "projMat";
	po_uniforms.mList[U_CAMERA_POS] = "eyePos";

	TexProp texArray(TEX_2D_ARRAY_FILE);
	texArray.SetData("tex_array");
//	texArray.SetFile("tex/sequence0.bmp");
//	texArray.SetFile("tex/sequence1.bmp");
	texArray.SetParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texArray.SetParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texArray.SetParam(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texArray.SetParam(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	mViewRenderer->GetNewObject(PHONG_OBJ, "cube", po_uniforms)
			->ImportObj("obj3d/cube", 1.0f)
			->AttachShader(vs, fs, "sh_tex_array")
			->AttachTexture(texArray, "s_texArray");

	mViewRenderer->SetBackgroundColor(glm::vec3(0.1f));


}

void TextureArrayView::OnStep() {
	Shader_Mgr.GetShader("sh_tex_array")
			->SetUniformData("isTouch", mViewRenderer->GetTouch() ? 1.0f : 0.0f);
	mViewRenderer->RenderFrame();
}


