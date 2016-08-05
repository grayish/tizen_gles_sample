#ifndef __BASIC_OBJECT_MGR__
#define __BASIC_OBJECT_MGR__

#define Object_Mgr BasicObjectMgr::Inst()

#include <map>
#include <basic/obj/basic_object.h>

#include "basic/basic_type.h"
#include "basic/basic_singleton.hpp"

class BasicObject;

class BasicCamera;

class ABasicMap;

class BasicObjectMgr : public BasicSingleton<BasicObjectMgr> {
private:
	std::map<std::string, BasicObject *> mObjectList;
	BasicObject *mFocusObject;

public:
	/**
	 * @brief Constructor for the class BasicObjectMgr
	 */
	BasicObjectMgr();

	/**
	 * @brief Destructor for the class BasicObjectMgr
	 */
	~BasicObjectMgr();

	/**
	 * @brief Initiate all buffers of objects
	 */
	void InitAllObjectsBuffers();

	/**
	 * @brief
	 *
	 * @param[in] pointer of BasicCamera
	 */
	void SetAllCamera(BasicCamera *camera);

	/**
	 * @brief Delete all mapped objects
	 */
	void DeleteAll();

	/**
	 * @brief Rotate all objects with a quaternion
	 *
	 * @param[in] rot the quaternion
	 */
	void SetRotation(const glm::quat &rot);

	/**
	 * @brief Create a new object
	 *
	 * @param[in] type type of the object
	 * @param[in] type name name specified for the object
	 *
	 * @return a pointer of the object
	 */
	BasicObject *
	GetNewObject(const BasicObject_Type &type, const std::string &name, const ABasicMap &map, BasicCamera *camera);


};


#endif //__BASIC_OBJECT_MGR__
