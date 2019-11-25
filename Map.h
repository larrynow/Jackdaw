#pragma once
#ifndef JKMAP_H_
#define JKMAP_H_

#include"Character.h"
#include<string>

/*
A jkMap indicates all the actors and models at this time.
*/

class jkMap
{
public:

	jkMap() : m_pControlledCharacter(nullptr) { mEntities.clear(); };
	~jkMap() { delete m_pControlledCharacter; };

	inline void ControllCharacter(jkCharacter* pCharacter) { m_pControlledCharacter = pCharacter; }

	inline void AddEntity(jkEntity* entity) { mEntities.push_back(entity); };

	inline jkCharacter* GetControlledCharacter() { return m_pControlledCharacter; };

private:

	////////////////////////////////////////////
	// Map entities.

	jkCharacter* m_pControlledCharacter;

	std::vector<jkEntity*> mEntities;
};

#endif // !JKMAP_H_