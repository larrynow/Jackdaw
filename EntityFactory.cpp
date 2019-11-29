#include"EntityFactory.h"
std::unordered_map<std::string, EntityCreateFunc>jkEntityFactory::mEntityMap;

jkEntity* jkEntityFactory::GetEntity(const std::string entityName)
{
	if (mEntityMap.find(entityName) != mEntityMap.end())
	{
		return mEntityMap.at(entityName)();
	}
	else
	{
		// Don't registe first, return null.
		return nullptr;
	}
}
