#include "basic/mgr/basic_object_mgr.h"

#include <algorithm>
#include <basic/obj/transform_particles.h>
#include "basic/basic_utils.h"
#include "basic/obj/material_object.h"
#include "basic/obj/instanced_object.h"
#include "basic/obj/phong_object.h"
#include "basic/obj/simple_object.h"

BasicObjectMgr::BasicObjectMgr() :
		mObjectList(),
		mFocusObject(nullptr) {}

BasicObjectMgr::~BasicObjectMgr() {
	LOGI("MGR destruct");
	DeleteAll();
}

void BasicObjectMgr::SetupBufferObjects() {
	std::map<std::string, BasicObject *>::iterator iter = mObjectList.begin();
	for (; iter != mObjectList.end(); iter++) {
		iter->second->CreateVbo();
	}
}

void BasicObjectMgr::SetAllCamera(BasicCamera *camera) {
	std::map<std::string, BasicObject *>::iterator iter = mObjectList.begin();
	for (; iter != mObjectList.end(); iter++) {
		PhongObject *po = dynamic_cast<PhongObject *>(iter->second);
		if (po) po->mCamera = camera;
	}
}

BasicObject *BasicObjectMgr::GetNewObject(const BasicObject_Type &type, const std::string &name, const ABasicMap &map,
										  BasicCamera *camera) {
	if (mFocusObject) {
		mFocusObject->SetFocus(false);
	}

	BasicObject *ret = nullptr;

	switch (type) {
		case PHONG_OBJ :
			ret = new PhongObject(name, &map, camera);
			ret->SetFocus(true);
			mFocusObject = ret;
			break;
		case INST_OBJ :
			ret = new InstObject(name, &map, camera);
			ret->SetFocus(true);
			mFocusObject = ret;
			break;
		case TRANSFORM_OBJ :
			ret = new TransformParticles(name, &map);
			ret->SetFocus(true);
			mFocusObject = ret;
			break;
		case MATERIAL_OBJ :
			ret = new MaterialObject(name, &map, camera);
			ret->SetFocus(true);
			mFocusObject = ret;
			break;
		case SIMPLE_OBJ :
			ret = new SimpleObject(name, &map, camera);
			ret->SetFocus(true);
			mFocusObject = ret;
			break;
		default:
			break;
	}

	mObjectList[name] = ret;

	return ret;
}

void BasicObjectMgr::DeleteAll() {
	std::map<std::string, BasicObject *>::iterator iter = mObjectList.begin();
	for (; iter != mObjectList.end(); iter++) {
		delete iter->second;
	}
	mObjectList.clear();
}

void BasicObjectMgr::SetRotation(const glm::quat &rot) {
	std::map<std::string, BasicObject *>::iterator iter = mObjectList.begin();
	for (; iter != mObjectList.end(); iter++) {
		if (iter->second->mFocus) {
			iter->second->SetRotation(rot);
		}
	}
}

