#include "sample/view/mrt_view.h"

#include "basic/obj/basic_camera.h"
#include "basic/obj/basic_light.h"
#include "basic/obj/simple_object.h"
#include "basic/mgr/basic_light_mgr.h"
#include "basic/mgr/basic_texture_mgr.h"
#include "basic/mgr/basic_shader_mgr.h"

using namespace std;

#define MRT_WIDTH 480
#define MRT_HEIGHT 688

#define SH_MRT "sh_mrt"
#define SH_DRAW "sh_draw"

class MrtRenderer : public BasicRenderer {
private:
	GLuint mMrtFbo;
	GLuint mRenderBuffer;

public:


	MrtRenderer() :
			mMrtFbo(0),
			mRenderBuffer(0)
	{}

	virtual ~MrtRenderer() {}

	void InitFbo(GLuint* texIds) {
		GLenum none = GL_NONE;
		glGenRenderbuffers(1, &mRenderBuffer);
		check_gl_error("glGenRenderbuffers");
		glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
		check_gl_error("glBindRenderbuffer");
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, MRT_WIDTH, MRT_HEIGHT);
		check_gl_error("glRenderbufferStorage");

		glGenFramebuffers(1, &mMrtFbo);
		check_gl_error("glGenFramebuffers");

		glBindFramebuffer(GL_FRAMEBUFFER, mMrtFbo);
		check_gl_error("glBindFramebuffer");

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRenderBuffer);

		for(unsigned int i=0; i<4 ;i++) {
			glBindTexture(GL_TEXTURE_2D, texIds[i]);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
								   GL_COLOR_ATTACHMENT0 + i,
								GL_TEXTURE_2D, texIds[i], 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			check_gl_error("glFramebufferTexture2D");

		}
		LOGI("texid [%d, %d, %d, %d]", texIds[0],texIds[1],texIds[2],texIds[3]);

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, DrawBuffers);

		// Check FBO is ready to draw
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			LOGE("FrameBufferObject is not complete!");
		}

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	virtual void RenderFrame() {
		ComputeTick();

		glBindFramebuffer(GL_FRAMEBUFFER, mMrtFbo);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		Shader_Mgr.DrawObjects(SH_MRT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		Shader_Mgr.DrawObjects(SH_DRAW);
		glEnable(GL_CULL_FACE);

	}


};



MrtView::MrtView(void *data) : SampleView(data, false) {
	mViewRenderer = new MrtRenderer();

}

void MrtView::OnInit() {
	string mrt_vs = File_Loader.ReadFileToString("shader/view_mrt/mrt.vs");
	string mrt_fs = File_Loader.ReadFileToString("shader/view_mrt/mrt.fs");
	string draw_vs = File_Loader.ReadFileToString("shader/view_mrt/draw.vs");
	string draw_fs = File_Loader.ReadFileToString("shader/view_mrt/draw.fs");

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
			->ImportObj("obj3d/chess_tri", 2.0f)
			->AttachShader(mrt_vs, mrt_fs, SH_MRT)
			->AttachLight(POINT_LT, "point_light_1", lt_uniforms);

	Light_Mgr.GetLight("point_light_1")->mPosition = glm::vec3(0, 5.0f, 0);

	mViewRenderer->OffAutoRotate();

	mViewRenderer->GetCamera()->SetEye(glm::vec3(-15, 15, 15));

	TexProp texDiff(TEX_2D_PTR);
	texDiff.SetData("tex_diff", MRT_WIDTH, MRT_HEIGHT, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE);
	texDiff.SetPointer(nullptr);
	texDiff.SetFilter();

	TexProp texDraw(TEX_2D_PTR);
	texDraw.SetData("tex_draw", MRT_WIDTH, MRT_HEIGHT, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE);
	texDraw.SetPointer(nullptr);
	texDraw.SetFilter();

	TexProp texAmbi(TEX_2D_PTR);
	texAmbi.SetData("tex_ambi", MRT_WIDTH, MRT_HEIGHT, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE);
	texAmbi.SetPointer(nullptr);
	texAmbi.SetFilter();

	TexProp texAttn(TEX_2D_PTR);
	texAttn.SetData("tex_attn", MRT_WIDTH, MRT_HEIGHT, GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE);
	texAttn.SetPointer(nullptr);
	texAttn.SetFilter();

	BasicObject *obj;

	obj = mViewRenderer->GetNewObject(SIMPLE_OBJ, "diff", mo_uniforms)
			->AttachShader(draw_vs, draw_fs, SH_DRAW)
			->AttachTexture(texDiff, "s_tex");
	dynamic_cast<SimpleObject *>(obj)->Init(glm::vec2(0));

	obj = mViewRenderer->GetNewObject(SIMPLE_OBJ, "spec", mo_uniforms)
			->AttachShader(draw_vs, draw_fs, SH_DRAW)
			->AttachTexture(texDraw, "s_tex");
	dynamic_cast<SimpleObject *>(obj)->Init(glm::vec2(-1.0, 0));
	obj = mViewRenderer->GetNewObject(SIMPLE_OBJ, "ambi", mo_uniforms)
			->AttachShader(draw_vs, draw_fs, SH_DRAW)
			->AttachTexture(texAmbi, "s_tex");
	dynamic_cast<SimpleObject *>(obj)->Init(glm::vec2(0, -1.0));
	obj = mViewRenderer->GetNewObject(SIMPLE_OBJ, "attn", mo_uniforms)
			->AttachShader(draw_vs, draw_fs, SH_DRAW)
			->AttachTexture(texAttn, "s_tex");
	dynamic_cast<SimpleObject *>(obj)->Init(glm::vec2(-1.0, -1.0));

	GLuint texIds[4];
	texIds[0] = Texture_Mgr.GetTextureId("tex_diff");
	texIds[1] = Texture_Mgr.GetTextureId("tex_draw");
	texIds[2] = Texture_Mgr.GetTextureId("tex_ambi");
	texIds[3] = Texture_Mgr.GetTextureId("tex_attn");
	LOGI("texid [%d, %d, %d, %d]", texIds[0],texIds[1],texIds[2],texIds[3]);

	mViewRenderer->SetCurrShader(SH_DRAW);

	dynamic_cast<MrtRenderer *>(mViewRenderer)->InitFbo(texIds);

}

