#ifndef __BASIC_SHADER_MGR_H__
#define __BASIC_SHADER_MGR_H__

#define Shader_Mgr BasicShaderMgr::Inst()

#include <string>
#include <map>
#include <vector>

#include "basic/basic_singleton.hpp"

class BasicObject;

class BasicShader;

class BasicShaderMgr : public BasicSingleton<BasicShaderMgr> {
private:
	typedef std::pair<BasicShader *, bool> ShPair;
	typedef std::map<std::string, ShPair> ShList;
	typedef std::map<BasicShader *, std::vector<BasicObject *>> ShDictionary;

	ShList mShaderList;
	ShDictionary mShaderDic;

public:
	BasicShaderMgr();

	~BasicShaderMgr();

	void Init();

	void DeleteAll();

	void DrawObjects(const std::string &sh_name);

	void DrawObjects(BasicShader *sh);

	BasicShader *GetNewShader(const std::string &vert_sh, const std::string &frag_sh, const std::string &name);

	BasicShader *GetShader(const std::string &sh_name);

	void SetShaderObject(BasicShader *sh, BasicObject *obj);

	void SetShaderObject(const std::string &sh_name, BasicObject *obj);

	/*template <typename T>
	BasicObject *SetUniformData(BasicObject *obj, const std::string &u_str, const T &data);*/
//    void ActiveShader(BasicObject* obj);
	BasicShader *GetLastShader();

	void DrawAll();

	void SetShaderOnOff(const std::string &sh_name, const bool &onoff);
};

/*template<typename T>
BasicObject *BasicShaderMgr::SetUniformData(BasicObject *obj, const std::string &u_str, const T &data) {
    if(mShaderDic.find(obj) == mShaderDic.end()) {
        LOGE("BasicShaderMgr::SetUniformData - shader doesn't exist!");
    } else {
        mShaderDic[obj]->SetUniformData(u_str, data);
    }

    return obj;
}*/


#endif //__BASIC_SHADER_MGR_H__
