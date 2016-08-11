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
	/**
	 * @brief Constructor for the class BasicShaderMgr
	 */
	BasicShaderMgr();

	/**
	 * @brief Destructor for the class BasicShaderMgr
	 */
	virtual ~BasicShaderMgr();

	/**
	 * @brief Setup a shader manager
	 */
	void Setup();

	/**
	 * @brief Delete all mapped components
	 */
	void DeleteAll();

	/**
	 * @brief Draw objects linked with a shader
	 *
	 * @param[in] sh_name a name of the shader
	 */
	void DrawObjects(const std::string &sh_name);

	/**
	 * @brief Draw objects linked with a shader
	 *
	 * @param[in] sh a pointer of the shader
	 */
	void DrawObjects(BasicShader *sh);

	/**
	 * @brief Create a new shader
	 *
	 * @param[in] vert_sh a code of a vertex shader
	 * @param[in] frag_sh a code of a fragment shader
	 * @param[in] type name name specified for the shader
	 *
	 * @return a pointer of the shader
	 */
	BasicShader *GetNewShader(const std::string &vert_sh, const std::string &frag_sh, const std::string &name);

	/**
	 * @brief Get a shader object
	 *
	 * @param[in] sh_name a name specified for the shader object
	 *
	 * @return a pointer of the shader
	 */
	BasicShader *GetShader(const std::string &sh_name);

	/**
	 * @brief Link a shader with a object
	 *
	 * @param[in] sh a pointer of the shader
	 * @param[in] obj a pointer of the object
	 */
	void SetShaderObject(BasicShader *sh, BasicObject *obj);

	/**
	 * @brief Link a shader with a object
	 *
	 * @param[in] sh_name a name of the shader
	 * @param[in] obj a pointer of the object
	 */
	void SetShaderObject(const std::string &sh_name, BasicObject *obj);

	/**
	 * @brief Get a shader added recently
	 *
	 * @return a pointer of the shader
	 */
	BasicShader *GetLastShader();

	/**
	 * @brief Draw all objects linked with itself
	 */
	void DrawAll();

	/**
	 * @brief Turn a shader on or off
	 *
	 * @param[in] sh_name a name of the shader
	 */
	void SetShaderOnOff(const std::string &sh_name, const bool &onoff);
};


#endif //__BASIC_SHADER_MGR_H__
