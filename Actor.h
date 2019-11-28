#pragma once
#ifndef JKACTOR_H_
#define JKACTOR_H_

#include "Entity.h"
#include "Camera.h"

/*
Class jkActor : An actor is an entity that is self-controlled
(with AI or fixed behavior pattern).
*/

class jkActor : public jkEntity
{
public:
	
	jkActor(const VEC3& actorPosition, const VEC3& cameraPosition, float moveSpeed = 1.0f) : jkEntity(actorPosition),
		m_pCamera(new jkCamera(cameraPosition)), mMoveSpeed(moveSpeed) {};

	jkActor() : jkActor({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 10.f }) {};// Default actor camera position is behind.
	~jkActor() { delete m_pCamera; };

	virtual void AddMovement(const VEC3& direction, float movement);
	inline void MoveForward() { AddMovement(m_pCamera->GetFront(), mMoveSpeed); };
	inline virtual void MoveBack() { AddMovement(-m_pCamera->GetFront(), mMoveSpeed); };
	inline virtual void MoveLeft() { AddMovement(-m_pCamera->GetRight(), mMoveSpeed); };
	inline virtual void MoveRight() { AddMovement(m_pCamera->GetRight(), mMoveSpeed); };
	inline virtual void MoveUp() { AddMovement(m_pCamera->GetUp(), mMoveSpeed); };
	inline virtual void MoveDown() { AddMovement(-m_pCamera->GetUp(), mMoveSpeed); };

	inline jkCamera* GetCamera() { return m_pCamera; };
	inline void MakeViewMatrix(MAT4& mat) { m_pCamera->MakeViewMatrix(mat); };

private:

	/////////////////////////////////////
	// Actor status.

	//VEC3 mPosition;

	float mMoveSpeed;

	/////////////////////////////////////
	// Actor components.

	jkCamera* m_pCamera;
};

#endif // !JKACTOR_H_