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

	virtual ~BasicShaderMgr();

	void Init();

	void DeleteAll();

	void DrawObjects(const std::string &sh_name);

	void DrawObjects(BasicShader *sh);

	BasicShader *GetNewShader(const std::string &vert_sh, const std::string &frag_sh, const std::string &name);

	BasicShader *GetShader(const std::string &sh_name);

	void SetShaderObject(BasicShader *sh, BasicObject *obj);

	void SetShaderObject(const std::string &sh_name, BasicObject *obj);

	BasicShader *GetLastShader();

	void DrawAll();

	void SetShaderOnOff(const std::string &sh_name, const bool &onoff);
};


#endif //__BASIC_SHADER_MGR_H__
