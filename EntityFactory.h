#pragma once
#ifndef JKENTITYFACTORY_H_
#define JKENTITYFACTORY_H_

#include"Entity.h"
#include<unordered_map>
#include<string>

typedef class jkEntity* (*EntityCreateFunc)(void);

class jkEntityFactory
{
public:
	static class jkEntity* GetEntity(const std::string);
	static void RegisterEntity(const std::string entityClassName, EntityCreateFunc func);

private:
	static std::unordered_map<std::string, EntityCreateFunc> mEntityMap;
};

class jkEntityRegisterAction
{
public:
	jkEntityRegisterAction(const std::string entityClassname, EntityCreateFunc func)
	{
		jkEntityFactory::RegisterEntity(entityClassname, func);
	}
};

#endif // JKENTITYFACTORY_H_