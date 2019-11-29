#pragma once
#ifndef JKENTITY_H_
#define JKENTITY_H_

#include"Types.h"
#include"Mesh.h"
#include"EntityFactory.h"

/*
Class jkEntity : An entity is object that exist in a map.
jkEntity have a mesh and a transform.
*/

class jkEntity
{
public:

	jkEntity(const VEC3& position) : mPosition(position){ mMesh = new jkMesh(mPosition); }

	jkEntity() : jkEntity({0.f, 0.f, 0.f}) {}

	//Transform transform;
	inline jkMesh* GetMesh() { return mMesh; }
	std::string MeshPath;

protected:

	jkMesh* mMesh;

	VEC3 mPosition;
};

//#ifndef REG_ENTITY(jkEntity);


//#endif // !REG_ENTITY(jkEntity)

#endif // !JKENTITY_H_



