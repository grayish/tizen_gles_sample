#include "sample/view/dyn_env_mapping_view.h"

#include "basic/mgr/basic_object_mgr.h"
#include "basic/mgr/basic_shader_mgr.h"
#include "basic/mgr/basic_texture_mgr.h"
#include "basic/obj/basic_camera.h"

#define SH_FRAG_LT "sh_frag"
#define SH_TEX "sh_tex"
#define SH_NOR "sh_nor"
#define SH_COLOR "sh_color"
#define SH_CUBE "sh_cube"

using namespace std;
using namespace glm;

class DynamicCubeRenderer : public BasicRenderer {

private:
	Buint mDynCubeSize;
	Buint mDynCubeRbo;
	Buint mDynCubeFbos[6];

public:
	DynamicCubeRenderer() :
			BasicRenderer(),
			mDynCubeSize(),
			mDynCubeRbo(),
			mDynCubeFbos{0,} {}

	virtual ~DynamicCubeRenderer() {
		glDeleteRenderbuffers(1, &mDynCubeRbo);
		glDeleteFramebuffers(6, mDynCubeFbos);
	}

	void InitDynamicCubemap(const Buint &cubeTexId, const Buint &size) {
		LOGI("cube texture id : %d", cubeTexId);
		mDynCubeSize = size;
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexId);

		glGenRenderbuffers(1, &mDynCubeRbo);
		check_gl_error("glGenRenderbuffers");
		glBindRenderbuffer(GL_RENDERBUFFER, mDynCubeRbo);
		check_gl_error("glBindRenderbuffer");
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mDynCubeSize, mDynCubeSize);
		check_gl_error("glRenderbufferStorage");

		glGenFramebuffers(6, mDynCubeFbos);
		check_gl_error("glGenFramebuffers");

		for (int i = 0; i < 6; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, mDynCubeFbos[i]);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDynCubeRbo);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
								   (GLenum) (GL_TEXTURE_CUBE_MAP_POSITIVE_X) + i, cubeTexId, 0);

			// Check FBO is ready to draw
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				LOGE("FrameBufferObject is not complete!");
			}
		}
		check_gl_error("glFramebufferTexture2D");

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual void RenderFrame() {
		ComputeTick();

		BasicCamera dynCamera(2.0f, 2000.0f);
		dynCamera.SetEye(vec3(0));
		dynCamera.ComputePerspective(radians(90.0f), mDynCubeSize, mDynCubeSize);

		vec3 cameraDir[6] =
				{vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0),
				 vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1)};
		vec3 upDir[6] =
				{vec3(0, -1, 0), vec3(0, -1, 0), vec3(0, 0, 1),
				 vec3(0, 0, 1), vec3(0, -1, 0), vec3(0, -1, 0)};

		Object_Mgr.SetAllCamera(&dynCamera);
		Shader_Mgr.SetShaderOnOff(SH_CUBE, false);
		glViewport(0, 0, mDynCubeSize, mDynCubeSize);
		for (int i = 0; i < 6; i++) {
			dynCamera.SetUp(upDir[i]);
			dynCamera.SetAt(cameraDir[i]);

			glBindFramebuffer(GL_FRAMEBUFFER, mDynCubeFbos[i]);
			check_gl_error("glBindFramebuffer");

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			check_gl_error("glClear");

			Shader_Mgr.DrawAll();

		}

		Object_Mgr.SetAllCamera(mCamera);
		Shader_Mgr.SetShaderOnOff(SH_CUBE, true);
		glViewport(0, 0, mWidth, mHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (mIsAutoRotateEye) {
			mCamera->RotateAuto(mDeltaTime);
		}

		Object_Mgr.SetRotation(CalculateRot());

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		check_gl_error("glClear");

		Shader_Mgr.DrawAll();

	}

};


DynEnvMappingView::DynEnvMappingView(void *data) : SampleView(data, false) {
	mViewRenderer = new DynamicCubeRenderer();
}

void DynEnvMappingView::OnInit() {
	string teapot = File_Loader.ReadTxtFile("obj3d/teapot");
	string cube = File_Loader.ReadTxtFile("obj3d/cube");
	string plane = File_Loader.ReadTxtFile("obj3d/plane");
	string skybox = File_Loader.ReadTxtFile("obj3d/skybox");

	string cube_vs = File_Loader.ReadTxtFile("shader/view_env/env.vs");
	string cube_fs = File_Loader.ReadTxtFile("shader/view_env/env.fs");
	string color_vs = File_Loader.ReadTxtFile("shader/view_color/color.vs");
	string color_fs = File_Loader.ReadTxtFile("shader/view_color/color.fs");
	string tex_vs = File_Loader.ReadTxtFile("shader/view_tex/tex.vs");
	string tex_fs = File_Loader.ReadTxtFile("shader/view_tex/tex.fs");
	string f_vs = File_Loader.ReadTxtFile("shader/view_f_lit/f_lit.vs");
	string f_fs = File_Loader.ReadTxtFile("shader/view_f_lit/f_lit.fs");
	string nor_vs = File_Loader.ReadTxtFile("shader/view_nor/nor.vs");
	string nor_fs = File_Loader.ReadTxtFile("shader/view_nor/nor.fs");

	TexContainer textures[7];
	File_Loader.ReadTexture("tex/tex_skybox.jpg", textures[0]);
	File_Loader.ReadTexture("tex/tizen_black.png", textures[1]);
	File_Loader.ReadTexture("tex/textj.png", textures[2]);
	File_Loader.ReadTexture("tex/tizen.png", textures[3]);
	File_Loader.ReadTexture("tex/tex_c_brick.bmp", textures[4]);
	File_Loader.ReadTexture("tex/tex_n_brick.bmp", textures[5]);
	File_Loader.ReadTexture("tex/tex_checked_tile.jpg", textures[6]);

	TexContainer dynCube[6];
	string DYN_CUBENAME = "dynCube";
	unsigned int DYN_CUBE_TEXSIZE = 720;
	for (int i = 0; i < 6; i++) {
		dynCube[i].filename = DYN_CUBENAME;
		dynCube[i].pixels = nullptr;
		dynCube[i].width = DYN_CUBE_TEXSIZE;
		dynCube[i].height = DYN_CUBE_TEXSIZE;
		dynCube[i].format = GL_RGB;
		dynCube[i].internalFormat = GL_RGB;
		dynCube[i].dataType = GL_UNSIGNED_BYTE;
	}

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


	string LT_POINT_1 = "lt_point_1";

	// skybox
	mViewRenderer->GetNewObject(PHONG_OBJ, "skybox", po_uniforms)
			->ImportObj(skybox, 2.0f)
			->AttachShader(tex_vs, tex_fs, SH_TEX)
			->AttachTexture(textures[0], GL_TEXTURE_2D, "s_tex0")
			->SetPosition(vec3(0));

	// surroundings
	mViewRenderer->GetNewObject(PHONG_OBJ, "cube1", po_uniforms)
			->ImportObj(cube, 0.5f)
			->AttachShader(f_vs, f_fs, SH_FRAG_LT)
			->AttachLight(POINT_LT, LT_POINT_1, lt_uniforms)
			->AttachTexture(textures[1], GL_TEXTURE_2D, "s_tex0")
			->SetPosition(vec3(20.0f, 0, 0));
	mViewRenderer->GetNewObject(PHONG_OBJ, "cube2", po_uniforms)
			->ImportObj(cube, 0.5f)
			->AttachShader(SH_FRAG_LT)
			->AttachLight(LT_POINT_1)
			->AttachTexture(textures[2], GL_TEXTURE_2D, "s_tex0")
			->SetPosition(vec3(0, 20.0f, 0));
	mViewRenderer->GetNewObject(PHONG_OBJ, "cube3", po_uniforms)
			->ImportObj(cube, 0.5f)
			->AttachShader(SH_FRAG_LT)
			->AttachLight(LT_POINT_1)
			->AttachTexture(textures[3], GL_TEXTURE_2D, "s_tex0")
			->SetPosition(vec3(0, 0, 20.0f));
	mViewRenderer->GetNewObject(PHONG_OBJ, "cube_brick", po_uniforms)
			->ImportObj(cube, 0.5f)
			->AttachShader(nor_vs, nor_fs, SH_NOR)
			->AttachTexture(textures[4], GL_TEXTURE_2D, "s_tex0")
			->AttachTexture(textures[5], GL_TEXTURE_2D, "s_texNor")
			->AttachLight(LT_POINT_1)
			->SetPosition(vec3(0, 0, -20.0f));

	mViewRenderer->GetNewObject(PHONG_OBJ, "cube_color", po_uniforms)
			->ImportObj(cube, 0.5f)
			->AttachShader(color_vs, color_fs, SH_COLOR)
			->SetPosition(vec3(-20.0f, 0, 0));

	// ground
	mViewRenderer->GetNewObject(PHONG_OBJ, "ground", po_uniforms)
			->ImportObj(plane, 100.0f)
			->AttachShader(SH_FRAG_LT)
			->AttachLight(LT_POINT_1)
			->AttachTexture(textures[6], GL_TEXTURE_2D, "s_tex0")
			->SetPosition(vec3(0, -20.0f, 0));

	// dynamic obj
	mViewRenderer->GetNewObject(PHONG_OBJ, "teapot", po_uniforms)
			->ImportObj(teapot, 1.0f)
			->AttachShader(cube_vs, cube_fs, SH_CUBE)
			->AttachCubeTex(dynCube, "s_texCube");

	dynamic_cast<DynamicCubeRenderer *>(mViewRenderer)->InitDynamicCubemap(
			Texture_Mgr.GetTextureId(DYN_CUBENAME),
			DYN_CUBE_TEXSIZE
	);

}


