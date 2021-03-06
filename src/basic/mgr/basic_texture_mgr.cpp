#include "basic/mgr/basic_texture_mgr.h"

#include "basic/basic_utils.h"
#include "basic/basic_gl_set.h"
#include "basic/basic_shader.h"
#include "basic/tex/tex_prop.h"
#include "basic/tex/basic_texture.h"

BasicTextureMgr::BasicTextureMgr() :
		mTextureList(),
		mTextureData() {
}

BasicTextureMgr::~BasicTextureMgr() {
	LOGI("MGR destruct");
	DeleteAll();
}

void BasicTextureMgr::Setup() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	check_gl_error("glPixelStorei");
}

void BasicTextureMgr::DeleteAll() {
	std::map<BasicObject *, std::vector<BasicTexture *>>::iterator iter_data = mTextureData.begin();
	for (; iter_data != mTextureData.end(); iter_data++) {
		iter_data->second.clear();
	}
	mTextureData.clear();

	std::map<std::string, BasicTexture *>::iterator iter_list = mTextureList.begin();
	for (; iter_list != mTextureList.end(); iter_list++) {
		delete iter_list->second;
	}
	mTextureList.clear();
}

int BasicTextureMgr::SetObjectTexture(BasicObject *obj, const TexProp &tex, const std::string &uniform_name) {
	std::map<std::string, BasicTexture *>::iterator iter_list;
	iter_list = mTextureList.find(tex.mName);

	BasicTexture *basicTex(nullptr);
	if (iter_list == mTextureList.end()) {
		basicTex = new BasicTexture(tex, uniform_name);
		mTextureList[tex.mName] = basicTex;
	} else {
		basicTex = mTextureList[tex.mName];
	}

	mTextureData[obj].push_back(basicTex);

	return mTextureData[obj].size() - 1;
}

GLuint BasicTextureMgr::GetTextureId(const std::string &tex_str) {
	if (mTextureList.find(tex_str) == mTextureList.end()) {
		LOGE("The texture, %s, doesn't exist!", tex_str.c_str());
		return 0;
	}
	return mTextureList[tex_str]->mTexId;
}

void BasicTextureMgr::ActiveTextures(BasicShader *sh, BasicObject *obj) {
	std::map<BasicObject *, std::vector<BasicTexture *>>::iterator iter_data = mTextureData.find(obj);

	if (iter_data != mTextureData.end()) {
		std::vector<BasicTexture *>::iterator it = mTextureData[obj].begin();

		unsigned int activeSlot = 0;
		for (; it != mTextureData[obj].end(); it++) {
			glActiveTexture(GL_TEXTURE0 + activeSlot);
			check_gl_error("glActiveTexture");
			glBindTexture((*it)->mTarget, (*it)->mTexId);
			check_gl_error("glBindTexture");
			sh->SetUniformData((*it)->mUniformName, activeSlot);
			activeSlot++;
		}
	}
}

void BasicTextureMgr::DeactiveTextures(BasicObject *obj) {
	std::map<BasicObject *, std::vector<BasicTexture *>>::iterator iter_data = mTextureData.find(obj);

	if (iter_data != mTextureData.end()) {
		std::vector<BasicTexture *>::iterator it = mTextureData[obj].begin();

		unsigned int activeSlot = 0;
		for (; it != mTextureData[obj].end(); it++) {
			glBindTexture((*it)->mTarget, 0);
			check_gl_error("glBindTexture");
			activeSlot++;
		}
	}
}
