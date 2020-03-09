#pragma once
#ifndef JKSETTING
#define JKSETTING

#include<string>
#include<tinyxml2.h>
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

	static void ReadLevelSetting(const std::string& setting_file);

	// Use entityFactory create entity.

};

#endif // !JKSETTING
