#pragma once
#ifndef JKENTITY_H_
#define JKENTITY_H_

#include"Types.h"
#include"Model.h"
#include"Transform.h"
#include"EntityFactory.h"

/*
Class jkEntity : An entity is object that exist in a map.
jkEntity have a model and a transform.
*/

class jkEntity
{
public:

	jkEntity(const VEC3& position) : m_pModel(nullptr), mTransform(position){}

	jkEntity() : jkEntity({0.f, 0.f, 0.f}) {}

	inline jkModel* GetModel() { return m_pModel; }
	inline jkTransform& GetTransform() { return mTransform; }

protected:

	jkModel* m_pModel;

	jkTransform mTransform;

};

//#ifndef REG_ENTITY(jkEntity);


//#endif // !REG_ENTITY(jkEntity)

#endif // !JKENTITY_H_



