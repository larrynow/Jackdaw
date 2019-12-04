#pragma once
#ifndef JKENTITYFACTORY_H_
#define JKENTITYFACTORY_H_

#include"Entity.h"
#include<unordered_map>
#include<string>

#define REG_ENTITY(entityClassName)\
jkEntityRegisterAction jkRegisterAction##entityClassName\
(#entityClassName, [](){return new entityClassName();})

class jkEntity;
typedef jkEntity* (*EntityCreateFunc)(void);

class jkEntityFactory
{
public:
	~jkEntityFactory() { delete m_pEntityMap; }
	static jkEntity* GetEntity(const std::string entityName);
	static void RegisterEntity(std::string entityClassName, EntityCreateFunc func);
	static std::unordered_map<std::string, EntityCreateFunc>* GetEntityMapPtr();

private:
	static std::unordered_map<std::string, EntityCreateFunc>* m_pEntityMap;
};

class jkEntityRegisterAction
{
public:
	jkEntityRegisterAction(std::string entityClassname, EntityCreateFunc func);
	
};
#endif // JKENTITYFACTORY_H_