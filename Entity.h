#pragma once
#ifndef JKENTITY_H_
#define JKENTITY_H_

#include"Types.h"
#include"Model.h"
#include"EntityFactory.h"

/*
Class jkEntity : An entity is object that exist in a map.
jkEntity have a mesh and a transform.
*/

class jkEntity
{
public:

	jkEntity(const VEC3& position) : mPosition(position){ m_pModel = new jkModel(position); }

	jkEntity() : jkEntity({0.f, 0.f, 0.f}) {}

	//Transform transform;
	inline jkModel* GetModel() { return m_pModel; }
	std::string ModelPath;

	inline void SetPosition(const VEC3& pos) { mPosition = pos; m_pModel->SetPosition(pos); }

protected:

	jkModel* m_pModel;

	VEC3 mPosition;
};

//#ifndef REG_ENTITY(jkEntity);


//#endif // !REG_ENTITY(jkEntity)

#endif // !JKENTITY_H_



