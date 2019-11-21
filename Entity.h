#pragma once
#ifndef JKENTITY_H_
#define JKENTITY_H_

#include"Types.h"
#include"Mesh.h"
#include"EntityFactory.h"

#define REG_ENTITY(entityClassName)\
jkEntityRegisterAction jkRegisterAction##entityClassName\
(#entityClassName, [](){return new entityClassName();})

/*
Class jkEntity : An entity is object that exist in a map.
jkEntity have a mesh and a transform.
*/

class jkEntity
{
public:
	Transform transform;
private:
	jkMesh* mMesh;
};

#ifndef REG_ENTITY(jkEntity)
REG_ENTITY(jkEntity);
#endif // !REG_ENTITY(jkEntity)

#endif // !JKENTITY_H_



