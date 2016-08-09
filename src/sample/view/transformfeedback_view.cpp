#include "sample/view/transformfeedback_view.h"

#include "basic/obj/transform_particles.h"

#include "basic/tex/tex_noise.h"

using namespace std;



TransformFeedbackView::TransformFeedbackView(void *data) :
		SampleView(data, true) ,
		mParticle(nullptr) {
}

void TransformFeedbackView::OnInit() {

	string DRAW_SH = "tf_draw";
	string vs = File_Loader.ReadTxtFile("shader/view_tf/tf_draw.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_tf/tf_draw.fs");

	BasicMap<TransformObj_U_Elem> tf_uniforms;
	tf_uniforms.mList[U_TIME] = "u_time";
	tf_uniforms.mList[U_ACCEL] = "u_acceleration";
	tf_uniforms.mList[U_COLOR] = "u_color";
	tf_uniforms.mList[U_EMISSION_RATE] = "u_emissionRate";

	TexProp smoke(TEX_2D_FILE, "tex/smoke.tga");

	TexNoise noiseTex(128, 50.0f, "noise_tex");

	BasicObject *obj =
	mViewRenderer->GetNewObject(TRANSFORM_OBJ, "particle", tf_uniforms)
			->AttachShader(vs, fs, DRAW_SH)
			->AttachTexture(smoke, "s_tex")
			->AttachTexture(noiseTex, "s_noiseTex");

	string PARTICLE_SH = "tf_particle";
	string vsParticle = File_Loader.ReadTxtFile("shader/view_tf/tf_particle.vs");
	string fsParticle = File_Loader.ReadTxtFile("shader/view_tf/tf_particle.fs");

	const char *feedbackVaryings[5] =
			{
					"v_position",
					"v_velocity",
					"v_size",
					"v_curtime",
					"v_lifetime"
			};

	mParticle = dynamic_cast<TransformParticles*>(obj);
	mParticle->Attatch_TF_Shader(vsParticle, fsParticle, feedbackVaryings, PARTICLE_SH);

	mViewRenderer->SetCurrShader(DRAW_SH);



	/*InitTransformFeedback(vsParticle, fsParticle, feedbackVaryings);
	mNoiseTexId = Create3DNoiseTexture(128, 50.0);
	LOGI("3D Noise Texture ID : %d", mNoiseTexId);

	InitParticles();

	string vs = File_Loader.ReadTxtFile("shader/view_tf/tf_draw.vs");
	string fs = File_Loader.ReadTxtFile("shader/view_tf/tf_draw.fs");

	TexProp tex(TEX_2D_FILE);
	File_Loader.ReadTexture("tex/smoke.tga", tex);*/


}

void TransformFeedbackView::OnStep() {
	mParticle->UpdateParticles();
	SampleView::OnStep();
}
