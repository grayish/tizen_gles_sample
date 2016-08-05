#include "sample/view/shadow_view.h"

#include "basic/basic_file_loader.h"

#include "basic/mgr/basic_shader_mgr.h"
#include "basic/mgr/basic_texture_mgr.h"
#include "basic/mgr/basic_object_mgr.h"

#include "basic/obj/phong_object.h"
#include "basic/obj/basic_camera.h"

using namespace std;
using namespace glm;

#define SHADOW_TEX_SIZE 1024
#define SH_SHADOW_MAP "sh_shadow"
#define SH_FRAG_SHADOW "sh_frag_shadow"

class ShadowRenderer : public BasicRenderer {
private:
	Buint mShadowWidth;
	Buint mShadowHeight;
	Buint mShadowFbo;

public:
	ShadowRenderer() :
			BasicRenderer(),
			mShadowWidth(),
			mShadowHeight(),
			mShadowFbo(0) {}

	virtual ~ShadowRenderer() {
		glDeleteFramebuffers(1, &mShadowFbo);
	}

	void InitShadow(const Buint &shadowTexId, const Buint &size) {
		GLenum none = GL_NONE;

		mShadowWidth = size;
		mShadowHeight = size;

		// Generate Frame buffer and bind texture.
		// Specify the attachments of FBOs by using glFramebufferTexture2D(), glDrawbuffers(), etc.
		// Frame buffer generate and bind depth and texture
		glGenFramebuffers(1, &mShadowFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, mShadowFbo);

		// Setup for depth only FBOs
		glReadBuffer(none);
		glDrawBuffers(1, &none);

		// Attach render buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexId, 0);

		// Check FBO is ready to draw
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			LOGE("FrameBufferObject is not complete!\n");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual void RenderFrame() {
		ComputeTick();

		glBindFramebuffer(GL_FRAMEBUFFER, mShadowFbo);
		//set viewport size as a size of depth texture
		glViewport(0, 0, mShadowWidth, mShadowHeight);

		//disable color rendering
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		//clear depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);

		//Draw shadow
		Shader_Mgr.DrawObjects(SH_SHADOW_MAP);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, mWidth, mHeight);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		if (mIsAutoRotateEye) {
			mCamera->RotateAuto(mDeltaTime);
		}

		Object_Mgr.SetRotation(CalculateRot());

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		check_gl_error("glClear");

		Shader_Mgr.DrawObjects(SH_FRAG_SHADOW);

	}

};


ShadowView::ShadowView(void *data) :
		SampleView(data, false) {
	mViewRenderer = new ShadowRenderer();
}

void ShadowView::OnInit() {
	//for generating shadow map
	string vsShadow = File_Loader.ReadTxtFile("shader/view_shadow2/shadow.vs");
	string fsShadow = File_Loader.ReadTxtFile("shader/view_shadow2/shadow.fs");

	//for rendering scene
	string vs = File_Loader.ReadTxtFile("shader/view_shadow2/pcf_book.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_shadow2/pcf_book.fs");

	string teapot = File_Loader.ReadTxtFile("obj3d/teapot");
	string plane = File_Loader.ReadTxtFile("obj3d/plane");

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
	string POINT_LIGHT_1 = "point_light_1";
	lt_uniforms.mList[U_PL_DIFFUSE] = "sourceDiff";
	lt_uniforms.mList[U_PL_AMBIENT] = "sourceAmbi";
	lt_uniforms.mList[U_PL_SPECULAR] = "sourceSpec";
	lt_uniforms.mList[U_PL_POS] = "lightPos";

	TexContainer shadowTex;
	string SHADOW_TEX_NAME = "shadowTex";
	string SHADOW_TEX_U_STR = "s_texShadow";
	shadowTex.filename = SHADOW_TEX_NAME;
	shadowTex.pixels = nullptr;
	shadowTex.width = SHADOW_TEX_SIZE;
	shadowTex.height = SHADOW_TEX_SIZE;
	shadowTex.format = GL_DEPTH_COMPONENT;
	shadowTex.internalFormat = GL_DEPTH_COMPONENT24;
	shadowTex.dataType = GL_UNSIGNED_INT;
	shadowTex.SetParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	shadowTex.SetParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	shadowTex.SetParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	shadowTex.SetParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	shadowTex.SetParam(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	shadowTex.SetParam(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	BasicObject *obj = mViewRenderer->GetNewObject(PHONG_OBJ, "plane", po_uniforms)
			->ImportObj(plane, 100.0f)
			->AttachShader(vs, fs, SH_FRAG_SHADOW)
			->AttachShader(vsShadow, fsShadow, SH_SHADOW_MAP)
			->AttachLight(POINT_LT, POINT_LIGHT_1, lt_uniforms)
			->AttachTexture(shadowTex, GL_TEXTURE_2D, SHADOW_TEX_U_STR)
			->SetPosition(vec3(0, -20.0f, 0));
	dynamic_cast<PhongObject *>(obj)->SetColor(vec3(0.7));

	mViewRenderer->GetNewObject(PHONG_OBJ, "teapot", po_uniforms)
			->ImportObj(teapot, 1.0f)
			->AttachShader(vs, fs, SH_FRAG_SHADOW)
			->AttachShader(vsShadow, fsShadow, SH_SHADOW_MAP)
			->AttachLight(POINT_LT, POINT_LIGHT_1, lt_uniforms)
			->AttachTexture(shadowTex, GL_TEXTURE_2D, SHADOW_TEX_U_STR);

	mViewRenderer->GetCamera()->SetEye(45, 45, 45);
	mViewRenderer->GetCamera()->SetAt(0, 0, 0);

	dynamic_cast<ShadowRenderer *>(mViewRenderer)->InitShadow(
			Texture_Mgr.GetTextureId(SHADOW_TEX_NAME),
			SHADOW_TEX_SIZE
	);
}
