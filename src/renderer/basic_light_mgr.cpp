#include "basic/obj/basic_light.h"
#include "basic/mgr/basic_light_mgr.h"
#include "basic/basic_utils.h"

BasicLightMgr::BasicLightMgr() :
		mLightList(),
		mLightData() {
	Init();
}

BasicLightMgr::~BasicLightMgr() {
	DeleteAll();
}

void BasicLightMgr::Init() {

}

void BasicLightMgr::DeleteAll() {
	std::map<std::string, BasicLight *>::iterator iter_list = mLightList.begin();
	std::map<BasicObject *, std::vector<BasicLight *>>::iterator iter_data = mLightData.begin();

	for (; iter_list != mLightList.end(); iter_list++) {
		delete iter_list->second;
	}
	for (; iter_data != mLightData.end(); iter_data++) {
		iter_data->second.clear();
	}

	mLightList.clear();
	mLightData.clear();

}

BasicLight *BasicLightMgr::GetNewLight(const BasicLight_Type &type, const std::string &name, const ABasicMap &map) {
	BasicLight *ret = nullptr;

	if (mLightList.find(name) != mLightList.end()) {
		LOGE("The name, %s, is already exist!", name.c_str());
		ret = mLightList.find(name)->second;
		return ret;
	}

	switch (type) {
		case POINT_LT :
			ret = new PointLight(name, &map);
			break;
		default :
			break;

	}

	mLightList[name] = ret;

	return ret;
}

void BasicLightMgr::PassUniforms(BasicShader *sh, BasicObject *obj, BasicCamera *cam) {
	if (mLightData.find(obj) != mLightData.end()) {
		std::vector<BasicLight *>::iterator it = mLightData[obj].begin();
		for (; it != mLightData[obj].end(); it++) {
			(*it)->SetLightUnforms(sh, cam);
		}
	}

}

void BasicLightMgr::SetObjectLight(BasicObject *obj, BasicLight *lt) {
	LOGI("The light, %s, is pushed", lt->mName.c_str());
	mLightData[obj].push_back(lt);
}

BasicLight *BasicLightMgr::GetLight(const std::string &lt_name) {
	if (mLightList.find(lt_name) == mLightList.end()) {
		LOGE("The light, %s, doesn't exist!'", lt_name.c_str());
	}
	return mLightList[lt_name];
}

