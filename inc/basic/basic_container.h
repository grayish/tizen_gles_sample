#ifndef __BASIC_UNIFORM_LIST_H__
#define __BASIC_UNIFORM_LIST_H__

#include <string>
#include <map>

class ABasicMap {
public:
	ABasicMap() {}

	virtual ~ABasicMap() {}
};

template<typename Enum_T>
class BasicMap : public ABasicMap {
public:
	std::map<Enum_T, std::string> mList;

	BasicMap() : mList() {}

	virtual ~BasicMap() {}
};

#endif //__BASIC_UNIFORM_LIST_H__
