#include "basic/basic_renderer.h"

#include <algorithm>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "basic/obj/basic_timer.h"
#include "basic/obj/basic_camera.h"
#include "basic/mgr/basic_light_mgr.h"
#include "basic/mgr/basic_texture_mgr.h"
#include "basic/mgr/basic_shader_mgr.h"
#include "basic/mgr/basic_object_mgr.h"
#include "basic/basic_gl_set.h"

using namespace std;
using namespace glm;

BasicRenderer::BasicRenderer() :
		mTimer(nullptr),
		mCamera(nullptr),
		mCurrShader(nullptr),
		mWidth(0),
		mHeight(0),
		mDeltaTime(0),
		mTotalTime(0),
		mIsAutoRotateEye(true),
		mIsFill(true),
		mIsTouchOn(false),
		mTouchPoint(0),
		mBgColor(0) {
		mTimer = new BasicTimer();
		mCamera = new BasicCamera();
}

BasicRenderer::~BasicRenderer() {
	LOGI("renderer destructor ");
	delete mTimer;
	delete mCamera;
	Object_Mgr.release_Instance();
	Texture_Mgr.release_Instance();
	Light_Mgr.release_Instance();
	Shader_Mgr.release_Instance();
}

void BasicRenderer::SetStates() const {
	glClearColor(mBgColor.x, mBgColor.y, mBgColor.z, 0.0);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void BasicRenderer::LogInfo() const {
	print_gl_string("Version", GL_VERSION);
	print_gl_string("Vendor", GL_VENDOR);
	print_gl_string("Renderer", GL_RENDERER);
	print_gl_string("Extensions", GL_EXTENSIONS);
	print_gl_string("GLSLversion", GL_SHADING_LANGUAGE_VERSION);
}

void BasicRenderer::ComputeTick() {
	static double lastTick = 0;
	double currTick = mTimer->GetElapsedTime();
	mDeltaTime = currTick - lastTick;
	lastTick = currTick;
	mTotalTime += mDeltaTime;
	//LOGI("Tick: %f", mDeltaTime);
}

vec3 BasicRenderer::GetArcballVector(const vec2 &point) const {
	const float RADIUS = 1.0;

	vec3 P = vec3(1.0 * point.x / mWidth * 2 - 1.0,
				  1.0 * point.y / mHeight * 2 - 1.0,
				  0);
	P.y = -P.y;

	float OP_squared = P.x * P.x + P.y * P.y;
	if (OP_squared <= RADIUS * RADIUS)
		P.z = sqrt(RADIUS * RADIUS - OP_squared); // Pythagore
	else
		P = glm::normalize(P); // nearest point

	return P;
}

glm::quat BasicRenderer::CalculateRot() {
	static quat startRotQuat = quat_cast(mat3(1)); // focus오브젝트가 변경시 초기화 코드 넣어야함
	static quat lastRotQuat = startRotQuat;
	static vec2 ancPts = mTouchPoint;    //mTouchPoint
	static bool isUpdateAnc = false;

	if (mIsTouchOn) {
		if (!isUpdateAnc) {
			ancPts = mTouchPoint;
			isUpdateAnc = true;
			LOGI("CalculateRot() Anchor Pts Updated");
		}
		else {
			if (ancPts.x != mTouchPoint.x || ancPts.y != mTouchPoint.y) {
				// Get the vectors on the arcball
				vec3 va = GetArcballVector(ancPts);
				vec3 vb = GetArcballVector(mTouchPoint);

				// Get the rotation axis and the angle between the vector
				float angle = acos(glm::min(1.0f, dot(va, vb))) * 2.0f;

				vec3 axisInCameraSpace = normalize(cross(va, vb));

				mat4 cameraToObjectSpace = inverse(mCamera->GetViewMat());
				vec3 axisInObjectSpace = normalize(mat3(cameraToObjectSpace) * axisInCameraSpace);

				quat curRotQuat = angleAxis(angle, axisInObjectSpace);
				lastRotQuat = normalize(curRotQuat * startRotQuat);
			}
		}
	}
	else {
		startRotQuat = lastRotQuat;
		isUpdateAnc = false;
	}

	return lastRotQuat;
}

/****************************
 **** Interface functions ***
 ****************************/
void BasicRenderer::Initialize() {
	LOGI("BasicRenderer is initialized.");
	LogInfo();
	mTimer->InitTimer();
	SetStates();
	Object_Mgr.InitAllObjectsBuffers();
	Texture_Mgr.Init();
	Light_Mgr.Init();
	if (!mCurrShader) {
		mCurrShader = Shader_Mgr.GetLastShader();
	}
}

void BasicRenderer::SetViewPort(int w, int h) {
	LOGI("%d, %d", w, h);
	mWidth = w;
	mHeight = h;
	mTouchPoint = vec2(w/2.0f, h/2.0f);
	glViewport(0, 0, w, h);
	check_gl_error("glViewport");

	mCamera->ComputePerspective(radians(60.0f), w, h);
}

void BasicRenderer::RenderFrame() {
	ComputeTick();

	if (mIsAutoRotateEye) {
		//LOGI("mIsAutoRotateEye : %d, %f", mIsAutoRotateEye, mDeltaTime);
		mCamera->RotateAuto(mDeltaTime);
	}

	Object_Mgr.SetRotation(CalculateRot());

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	check_gl_error("glClear");

	if (mCurrShader)
		Shader_Mgr.DrawObjects(mCurrShader);

}

BasicObject *BasicRenderer::GetNewObject(BasicObject_Type type, const std::string &name, const ABasicMap &map) {
	return Object_Mgr.GetNewObject(type, name, map, mCamera);
}

void BasicRenderer::SetCurrShader(const std::string &sh_name) {
	mCurrShader = Shader_Mgr.GetShader(sh_name);

}

glm::vec2 BasicRenderer::GetScreenTouchPoint() const{
	return glm::vec2(2.0f * mTouchPoint.x / (float)mWidth - 1.0f,
					 1.0f - 2.0f * mTouchPoint.y / (float)mHeight );
}

