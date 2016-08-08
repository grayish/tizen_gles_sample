#include "sample/sample_view.h"

#include "gles_sample.h"
#include "basic/obj/basic_camera.h"

ELEMENTARY_GLVIEW_GLOBAL_DEFINE()

SampleView::SampleView(void *data, bool createDefaultRenderer) :
		mGLView(nullptr),
		mViewRenderer(nullptr) {
	mGLView = CreateView(data);
	if (createDefaultRenderer) {
		mViewRenderer = new BasicRenderer();
	}
}

SampleView::~SampleView() {
	LOGI_ENTRY;
	Release();
	LOGI_EXIT;
}

void SampleView::Release() {
	if (mViewRenderer) {
		delete mViewRenderer;
		mViewRenderer = nullptr;
		evas_object_del(mGLView);
	}
}

static void
mouse_down_cb(void *data, Evas *e, Evas_Object *obj, void *event_info) {
	appdata_s *ad = static_cast<appdata_s *>(evas_object_data_get(obj, "ad"));
	SampleView *sv = static_cast<SampleView *>(evas_object_data_get(obj, "sv"));
	Evas_Event_Mouse_Down *ev = static_cast<Evas_Event_Mouse_Down *>(event_info);

	sv->TouchOn();
	sv->SetTouchPoint(static_cast<float>(ev->canvas.x), static_cast<float>(ev->canvas.y));

	ad->mouse_down = EINA_TRUE;
}

static void
mouse_move_cb(void *data, Evas *e, Evas_Object *obj, void *event_info) {
	SampleView *sv = static_cast<SampleView *>(evas_object_data_get(obj, "sv"));
	Evas_Event_Mouse_Move *ev = static_cast<Evas_Event_Mouse_Move *>(event_info);

	sv->SetTouchPoint(static_cast<float>(ev->cur.canvas.x), static_cast<float>(ev->cur.canvas.y));
}

static void
mouse_up_cb(void *data, Evas *e, Evas_Object *obj, void *event_info) {
	appdata_s *ad = static_cast<appdata_s *>(data);
	SampleView *sv = static_cast<SampleView *>(evas_object_data_get(obj, "sv"));

	sv->TouchOff();
	ad->mouse_down = EINA_FALSE;
}


static void
resize_glview(Evas_Object *obj) {
	LOGI_ENTRY;
	appdata_s *ad = static_cast<appdata_s *>(evas_object_data_get(obj, "ad"));
	elm_glview_size_get(obj, &ad->glview_w, &ad->glview_h);
	SampleView *sv = static_cast<SampleView *>(evas_object_data_get(obj, "sv"));
	sv->OnWindowUpdate(ad->glview_w, ad->glview_h);
	LOGI_EXIT;
}

static void
draw_glview(Evas_Object *obj) {
	SampleView *sv = static_cast<SampleView *>(evas_object_data_get(obj, "sv"));
	sv->OnStep();
	glFlush();
}

static void
init_glview(Evas_Object *obj) {
	LOGI_ENTRY;
	SampleView *sv = static_cast<SampleView *>(evas_object_data_get(obj, "sv"));
	if (sv == nullptr) {
		LOGE("[sv] has null ptr!");
		return;
	}

	sv->Initialize();
	LOGI_EXIT;
}

static void
del_glview(Evas_Object *obj) {
	LOGI_ENTRY;
	SampleView *sv = static_cast<SampleView *>(evas_object_data_get(obj, "sv"));
	sv->Release();
	LOGI_EXIT;
}

static void
del_anim_cb(void *data, Evas *evas, Evas_Object *obj, void *event_info) {
	Ecore_Animator *ani = static_cast<Ecore_Animator *>(evas_object_data_get(obj, "ani"));
	ecore_animator_del(ani);
}

static Eina_Bool
anim_cb(void *data) {
	// notify glview should be updated again
	elm_glview_changed_set(static_cast<Evas_Object *>(data));
	return EINA_TRUE;
}

Evas_Object *
SampleView::CreateView(void *data) const {
	appdata_s *ad = static_cast<appdata_s *>(data);

	/* Create and initialize GLView */
	Evas_Object *glview = elm_glview_version_add(ad->navi, EVAS_GL_GLES_3_X);
	//Evas_Object* glview = elm_glview_add(ad->navi); 	//for GLES 2.0
	ELEMENTARY_GLVIEW_GLOBAL_USE(glview);

	evas_object_size_hint_align_set(glview, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(glview, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);


	/* Request a surface with alpha and a depth buffer */
	elm_glview_mode_set(glview, ELM_GLVIEW_DEPTH);

	/* The resize policy tells GLView what to do with the surface when it
	 * resizes. ELM_GLVIEW_RESIZE_POLICY_RECREATE will tell it to
	 * destroy the current surface and recreate it to the new size.
	 */
	elm_glview_resize_policy_set(glview, ELM_GLVIEW_RESIZE_POLICY_RECREATE);

	/* The render policy sets how GLView should render GL code.
	 * ELM_GLVIEW_RENDER_POLICY_ON_DEMAND will have the GL callback
	 * called only when the obj is visible.
	 * ELM_GLVIEW_RENDER_POLICY_ALWAYS would cause the callback to be
	 * called even if the obj were hidden.
	 */
	elm_glview_render_policy_set(glview, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);

	/* The initialize callback function gets registered here */
	elm_glview_init_func_set(glview, init_glview);

	/* The delete callback function gets registered here */
	elm_glview_del_func_set(glview, del_glview);

	/* The resize callback function gets registered here */
	elm_glview_resize_func_set(glview, resize_glview);

	/* The render callback function gets registered here */
	elm_glview_render_func_set(glview, draw_glview);

	/* This adds an animator so that the app will regularly
	 * trigger updates of the GLView using elm_glview_changed_set().
	 *
	 * NOTE: If you delete GL, this animator will keep running trying to access
	 * GL so this animator needs to be deleted with ecore_animator_del().
	 */
	Ecore_Animator *ani = ecore_animator_add(anim_cb, glview);
	evas_object_event_callback_add(glview, EVAS_CALLBACK_DEL, del_anim_cb, glview);
	//
	evas_object_event_callback_add(glview, EVAS_CALLBACK_MOUSE_DOWN, mouse_down_cb, ad);
	evas_object_event_callback_add(glview, EVAS_CALLBACK_MOUSE_UP, mouse_up_cb, ad);
	evas_object_event_callback_add(glview, EVAS_CALLBACK_MOUSE_MOVE, mouse_move_cb, ad);

	evas_object_data_set(glview, "ad", ad);
	evas_object_data_set(glview, "ani", ani);
	evas_object_data_set(glview, "sv", this);

	return glview;
}


void SampleView::OnWindowUpdate(int w, int h) const {
	mViewRenderer->SetViewPort(w, h);
}

void SampleView::Initialize() {
	this->OnInit();
	mViewRenderer->Initialize();
}

void SampleView::OnStep() {
	mViewRenderer->RenderFrame();
}

void SampleView::SetTouchPoint(float x, float y) const {
	mViewRenderer->SetTouchPoint(x, y);
}

void SampleView::TouchOn() const {
	mViewRenderer->TouchOn();
	mViewRenderer->OffAutoRotate();
}

void SampleView::TouchOff() const {
	mViewRenderer->OnAutoRotate();
	mViewRenderer->TouchOff();
}

void SampleView::ZoomIn(const float &vel) const {
	mViewRenderer->GetCamera()->ZoomIn(vel);
}

void SampleView::ZoomOut(const float &vel) const {
	mViewRenderer->GetCamera()->ZoomOut(vel);
}

void SampleView::MoveForward(const float &vel) const {
	mViewRenderer->GetCamera()->MoveForward(vel);
}

void SampleView::MoveBackward(const float &vel) const {
	mViewRenderer->GetCamera()->MoveBackward(vel);
}

void SampleView::MoveRight(const float &vel) const {
	mViewRenderer->GetCamera()->MoveRight(vel);
}

void SampleView::MoveLeft(const float &vel) const {
	mViewRenderer->GetCamera()->MoveLeft(vel);
}

void SampleView::MoveUp(const float &vel) const {
	mViewRenderer->GetCamera()->MoveUp(vel);
}

void SampleView::MoveDown(const float &vel) const {
	mViewRenderer->GetCamera()->MoveDown(vel);
}

void SampleView::RotateCamera(const float &dx, const float &dy) const {
	mViewRenderer->GetCamera()->Rotate(dx, dy);
}


