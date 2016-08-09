#include "basic/mgr/basic_shader_mgr.h"
#include "basic/basic_utils.h"
#include "basic/basic_shader.h"
#include "basic/obj/basic_object.h"

#include <algorithm>

BasicShaderMgr::BasicShaderMgr() :
		mShaderList(),
		mShaderDic() {
}

BasicShaderMgr::~BasicShaderMgr() {
	LOGE("MGR destruct");
	DeleteAll();
}

void BasicShaderMgr::Init() {

}

void BasicShaderMgr::DeleteAll() {
	LOGI_ENTRY;
	ShList::iterator iter_list = mShaderList.begin();

	for (; iter_list != mShaderList.end(); iter_list++) {
		delete iter_list->second.first;
	}
	mShaderList.clear();
	mShaderDic.clear();
	LOGI_EXIT;
}


BasicShader *
BasicShaderMgr::GetNewShader(const std::string &vert_sh, const std::string &frag_sh, const std::string &name) {
	LOGI("Get the shader, %s", name.c_str());
	BasicShader *ret = nullptr;

	if (mShaderList.find(name) != mShaderList.end()) {
		LOGE("The name, %s, is already exist!", name.c_str());
		return mShaderList[name].first;
	}

	ret = new BasicShader(name);
	ret->CreateProgram(vert_sh, frag_sh);

	if (!ret->GetProgram()) {
		LOGE("Could not create program.");
		delete ret;
		return nullptr;
	}

	mShaderList[name] = ShPair(ret, true);

	return ret;
}

void BasicShaderMgr::SetShaderObject(BasicShader *sh, BasicObject *obj) {
	mShaderDic[sh].push_back(obj);
}

BasicShader *BasicShaderMgr::GetShader(const std::string &sh_name) {
	if (mShaderList.find(sh_name) == mShaderList.end()) {
		LOGE("The shader, %s, doesn't exist!'", sh_name.c_str());
	}
	return mShaderList.find(sh_name)->second.first;
}

BasicShader *BasicShaderMgr::GetLastShader() {
	if (mShaderList.size() == 0) {
		LOGE("No shader exist!");
	}
	return mShaderList.rbegin()->second.first;
}

void BasicShaderMgr::DrawObjects(BasicShader *sh) {
	if (!sh) {
		LOGE("Null pointer input!");
		return;
	}
	sh->Use();
	std::vector<BasicObject *>::iterator it_obj = mShaderDic[sh].begin();
	for (; it_obj != mShaderDic[sh].end(); it_obj++) {
		(*it_obj)->SetShaderUniforms(sh);
		sh->PassUniforms();
		(*it_obj)->Draw();
	}

}

void BasicShaderMgr::DrawAll() {
	ShList::iterator it = mShaderList.begin();
	for (; it != mShaderList.end(); it++) {
		if (it->second.second) {
			DrawObjects(it->second.first);
		}
	}
}

void BasicShaderMgr::DrawObjects(const std::string &sh_name) {
	if (mShaderList[sh_name].second) {
		DrawObjects(mShaderList[sh_name].first);
	}
}

void BasicShaderMgr::SetShaderObject(const std::string &sh_name, BasicObject *obj) {
	if (mShaderList[sh_name].second) {
		SetShaderObject(mShaderList[sh_name].first, obj);
	}
}

void BasicShaderMgr::SetShaderOnOff(const std::string &sh_name, const bool &onoff) {
	mShaderList[sh_name].second = onoff;
}