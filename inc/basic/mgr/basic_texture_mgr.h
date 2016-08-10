#ifndef __BASIC_TEXTURE_MGR__
#define __BASIC_TEXTURE_MGR__

#define Texture_Mgr BasicTextureMgr::Inst()

#include <string>
#include <map>
#include <vector>

#include "basic/basic_singleton.hpp"
#include "basic/basic_type.h"
#include "basic/basic_gl_set.h"

class BasicTexture;

class BasicObject;

class BasicShader;

class TexProp;

class BasicTextureMgr : public BasicSingleton<BasicTextureMgr> {
private:
	std::map<std::string, BasicTexture *> mTextureList;
	std::map<BasicObject *, std::vector<BasicTexture *>> mTextureData;

public:
	/**
	 * @brief Constructor for the class BasicTextureMgr
	 */
	BasicTextureMgr();

	/**
	 * @brief Destructor for the class BasicTextureMgr
	 */
	virtual ~BasicTextureMgr();

	/**
	 * @brief Initiate a texture manager
	 */
	void Init();

	/**
	 * @brief Delete all mapped item of texture data
	 */
	void DeleteAll();

	/**
	 * @brief Set a texture of a BasicObject to input data
	 *
	 * @param[in] obj the pointer of BasicObject
	 * @param[in] tex data of the texture
	 * @param[in] uniform_name a name of a uniform variable of the texture
	 */
	int SetObjectTexture(BasicObject *obj, const TexProp &tex, const std::string &uniform_name);

	/**
	 * @brief Set ID of a texture
	 *
	 * @param[in] tex_str a name of the texture
	 */
	unsigned int GetTextureId(const std::string &tex_str);

	/**
	 * @brief Activate all textures of a object for a shader
	 **
	 * @param[in] sh a pointer of the shader
	 * @param[in] obj a pointer of the object
	 */
	void ActiveTextures(BasicShader *sh, BasicObject *obj);

	/**
	 * @brief Deactivate all textures of a object
	 **
	 * @param[in] obj a pointer of the object
	 */
	void DeactiveTextures(BasicObject *obj);
};


#endif //__BASIC_TEXTURE_MGR__
