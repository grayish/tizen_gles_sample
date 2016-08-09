#include "sample/view/transformfeedback_view.h"

#include "basic/obj/transform_particles.h"


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
	tf_uniforms.mList[U_TF_TIME] = "u_time";
	tf_uniforms.mList[U_TF_STEP] = "u_step";
	tf_uniforms.mList[U_TF_TOUCH_PT] = "u_touchPt";

	TexProp star(TEX_2D_FILE, "tex/star_blur.png");

	BasicObject *obj =
	mViewRenderer->GetNewObject(TRANSFORM_OBJ, "particle", tf_uniforms)
			->AttachShader(vs, fs, DRAW_SH)
			->AttachTexture(star, "s_tex");

	string PARTICLE_SH = "tf_particle";
	string vsParticle = File_Loader.ReadTxtFile("shader/view_tf/tf_particle.vs");
	string fsParticle = File_Loader.ReadTxtFile("shader/view_tf/tf_particle.fs");

	const char *feedbackVaryings[5] =
			{
					"v_pos",
					"v_color",
					"v_size",
					"v_life"
			};

	mParticle = dynamic_cast<TransformParticles*>(obj);
	mParticle->Attatch_TF_Shader(vsParticle, fsParticle, feedbackVaryings, 4, PARTICLE_SH);

	mViewRenderer->SetCurrShader(DRAW_SH);

}

void TransformFeedbackView::OnStep() {
	mParticle->UpdateParticles(mViewRenderer->GetScreenTouchPoint());
	SampleView::OnStep();
}
