#ifndef __BASIC_LIGHT_MGR_H__
#define __BASIC_LIGHT_MGR_H__

#define Light_Mgr BasicLightMgr::Inst()

#include <string>
#include <map>
#include <vector>

#include "basic/basic_type.h"
#include "basic/basic_singleton.hpp"

class BasicLight;

class BasicObject;

class BasicShader;

class BasicCamera;

class ABasicMap;

class BasicLightMgr : public BasicSingleton<BasicLightMgr> {
private:
	std::map<std::string, BasicLight *> mLightList;
	std::map<BasicObject *, std::vector<BasicLight *>> mLightData;

public:
	/**
	 * @brief Constructor for the class BasicLightMgr
	 */
	BasicLightMgr();

	/**
	 * @brief Destructor for the class BasicLightMgr
	 */
	virtual ~BasicLightMgr();

	/**
	 * @brief Initiate all light objects
	 */
	void Init();

	/**
	 * @brief Delete all mapped light objects
	 */
	void DeleteAll();

	/**
	 * @brief Create a new light object
	 *
	 * @param[in] type type of the light object
	 * @param[in] type name name specified for the light object
	 *
	 * @return a pointer of the light object
	 */
	BasicLight *GetNewLight(const BasicLight_Type &type, const std::string &name, const ABasicMap &map);

	BasicLight *GetLight(const std::string &lt_name);

	/**
	 * @brief Link a object with a light
	 *
	 * @param[in] obj a pointer of the object
	 * @param[in] lt a pointer of the light object
	 */
	void SetObjectLight(BasicObject *obj, BasicLight *lt);

	/**
	 * @brief Activate all lights linked with a object
	 */
	void PassUniforms(BasicShader *sh, BasicObject *obj, BasicCamera *cam);

};

#endif //__BASIC_LIGHT_MGR_H__
