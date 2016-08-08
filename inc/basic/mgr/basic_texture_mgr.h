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
	 * @brief Initiate the texture manager
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


//	int SetObjectCubeTex(BasicObject *obj, const TexProp *cubeTex, const std::string &uniform_name);


	unsigned int GetTextureId(const std::string &tex_str);

	/**
	 * @brief Activate all textures of a object
	 *
	 * @param[in] obj a pointer of the object
	 */
	void ActiveTextures(BasicShader *sh, BasicObject *obj);

	void DeactiveTextures(BasicObject *obj);
};


#endif //__BASIC_TEXTURE_MGR__
