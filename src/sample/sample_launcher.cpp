#include "sample/sample_launcher.h"
#include "gles_sample.h"

#include "sample/view/coloring_view.h"
#include "sample/view/texturing_view.h"
#include "sample/view/vertex_lighting_view.h"
#include "sample/view/frag_lighting_view.h"
#include "sample/view/env_mapping_view.h"
#include "sample/view/normal_mapping_view.h"
#include "sample/view/instance_view.h"
#include "sample/view/texture_array_view.h"
#include "sample/view/dyn_env_mapping_view.h"
#include "sample/view/shadow_view.h"
#include "sample/view/transformfeedback_view.h"
#include "sample/view/mrt_view.h"

SampleLauncher *SampleLauncher::instance = nullptr;

SampleLauncher::SampleLauncher() :
		curView(nullptr),
		mParent(nullptr) {
}

SampleLauncher::~SampleLauncher() {
	LOGE("destruct SampleLauncher");
	LOGI_ENTRY;
	if (instance)
		delete instance;
	LOGI_EXIT;
}

SampleLauncher *SampleLauncher::GetInstance() {
	if (!instance) {
		instance = new SampleLauncher();
	}
	return instance;
}

void SampleLauncher::Release() {
	if (curView) {
		delete curView;
		curView = nullptr;
	}

	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

Evas_Object *SampleLauncher::InitSampleView(void *data) {
	LOGI_ENTRY;
	appdata_s *ad = static_cast<appdata_s *>(data);
	int sampleNum = ad->cur_sample_num;

	mParent = ad->win;

	if (curView) {
		delete curView;
		curView = nullptr;
	}

	switch (sampleNum) {
		case 0:
			break;
		case 1:
			curView = new ColoringView(ad);
			break;
		case 2:
			curView = new TexturingView(ad);
			break;
		case 3:
			curView = new VertexLightingView(ad);
			break;
		case 4:
			curView = new FragLightingView(ad);
			break;
		case 5:
			curView = new EnvMappingView(ad);
			break;
		case 6:
			curView = new NormalMappingView(ad);
			break;
		case 7:
			curView = new InstanceView(ad);
			break;
		case 8:
			curView = new TextureArrayView(ad);
			break;
		case 9:
			curView = new DynEnvMappingView(ad);
			break;
		case 10:
			curView = new ShadowView(ad);
			break;
		case 11:
			curView = new TransformFeedbackView(ad);
			break;
		case 12:
			curView = new MrtView(ad);
			break;
		default:
			break;
	}

	LOGI_EXIT;
	return curView->GetGLView();
}

