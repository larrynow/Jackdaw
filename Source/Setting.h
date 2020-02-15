#pragma once
#ifndef JKSETTING
#define JKSETTING

#include<string>
#include"EntityFactory.h"

struct SettingItem
{
	std::string itemName;
	std::string itemValue;

	SettingItem* subItem;
};

class jkSetting
{

public:

	inline static void ReadSetting() {}

	// Use entityFactory create entity.

};

#endif // !JKSETTING
