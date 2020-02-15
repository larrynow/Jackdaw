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
	
	jkActor(const VEC3& actorPosition, const VEC3& cameraPosition, float moveSpeed = 10.0f) : jkEntity(actorPosition),
		m_pCamera(new jkCamera(cameraPosition)), mMoveSpeed(moveSpeed) {};

	jkActor() : jkActor({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 10.f }) {};// Default actor camera position is behind.
	~jkActor() { delete m_pCamera; };

	virtual void AddMovement(const VEC3& direction, float movement);
	
	virtual void Move(const VEC3& direction, float speed);
	inline void MoveForward() { Move(m_pCamera->GetFront(), mMoveSpeed); };
	inline virtual void MoveBack() { Move(-m_pCamera->GetFront(), mMoveSpeed); };
	inline virtual void MoveLeft() { Move(-m_pCamera->GetRight(), mMoveSpeed); };
	inline virtual void MoveRight() { Move(m_pCamera->GetRight(), mMoveSpeed); };
	inline virtual void MoveUp() { Move(m_pCamera->GetUp(), mMoveSpeed); };
	inline virtual void MoveDown() { Move(-m_pCamera->GetUp(), mMoveSpeed); };

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