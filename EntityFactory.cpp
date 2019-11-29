#include"EntityFactory.h"

std::unordered_map<std::string, EntityCreateFunc>* jkEntityFactory::m_pEntityMap = nullptr;

jkEntity* jkEntityFactory::GetEntity(const std::string entityName)
{
	if (m_pEntityMap->find(entityName) != m_pEntityMap->end())
	{
		return m_pEntityMap->at(entityName)();
	}
	else
	{
		// Have not registe yet, return null.
		return nullptr;
	}
}

void jkEntityFactory::RegisterEntity(std::string entityClassName, EntityCreateFunc func)
{
	jkEntityFactory::GetEntityMapPtr()->insert({entityClassName, func});
};

std::unordered_map<std::string, EntityCreateFunc>* jkEntityFactory::GetEntityMapPtr()
{
	if (!m_pEntityMap)
		m_pEntityMap = new std::unordered_map<std::string, EntityCreateFunc>;

	return m_pEntityMap;
}

jkEntityRegisterAction::jkEntityRegisterAction(std::string entityClassName, EntityCreateFunc func)
{
	jkEntityFactory::RegisterEntity(entityClassName, func);
};
