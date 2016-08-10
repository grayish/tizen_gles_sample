#ifndef __BASIC_UNIFORM_LIST_H__
#define __BASIC_UNIFORM_LIST_H__

#include <string>
#include <map>

class ABasicMap {
public:
	/**
	 * @brief Constructor for the class ABasicMap
	 */
	ABasicMap() {}

	/**
	 * @brief Destructor for the class ABasicMap
	 */
	virtual ~ABasicMap() {}
};

template<typename Enum_T>
class BasicMap : public ABasicMap {
public:
	std::map<Enum_T, std::string> mList;

	/**
	 * @brief Constructor for the class BasicMap
	 */
	BasicMap() : mList() {}

	/**
	 * @brief Destructor for the class BasicMap
	 */
	virtual ~BasicMap() {}
};

#endif //__BASIC_UNIFORM_LIST_H__
