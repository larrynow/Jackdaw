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
	
	jkActor(const VEC3& actorPosition, const VEC3& cameraPosition, 
		float moveSpeed = 10.f, float rotateSpeed = 1.f) 
		: jkEntity(actorPosition),
		m_pCamera(new jkCamera(cameraPosition)), 
		mMoveSpeed(moveSpeed), mRotateSpeed(rotateSpeed) {};

	jkActor() : jkActor({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 10.f }) {};// Default actor camera position is behind.
	~jkActor() { delete m_pCamera; };

	virtual void AddMovement(const VEC3& direction, float movement);
	
	virtual void Move(const VEC3& direction, float speed);
	inline virtual void MoveForward(float scaling) { Move(m_pCamera->GetFront(), scaling*mMoveSpeed); };
	inline virtual void MoveBack(float scaling) { Move(-m_pCamera->GetFront(), scaling*mMoveSpeed); };
	inline virtual void MoveLeft(float scaling) { Move(-m_pCamera->GetRight(), scaling*mMoveSpeed); };
	inline virtual void MoveRight(float scaling) { Move(m_pCamera->GetRight(), scaling*mMoveSpeed); };
	inline virtual void MoveUp(float scaling) { Move(m_pCamera->GetUp(), scaling*mMoveSpeed); };
	inline virtual void MoveDown(float scaling) { Move(-m_pCamera->GetUp(), scaling*mMoveSpeed); };

	virtual void RotatePitch(float value); 
	virtual void RotateYaw(float value); 
	virtual void RotateRoll(float value); 

	inline jkCamera* GetCamera() { return m_pCamera; };
	inline void MakeViewMatrix(MAT4& mat) { m_pCamera->MakeViewMatrix(mat); };

	inline virtual void ZoomUp(float value) { m_pCamera->SetFOV(Clamp(m_pCamera->GetFOV()*value, 30.f, 90.f)); }

private:

	/////////////////////////////////////
	// Actor status.

	//VEC3 mPosition;

	float mMoveSpeed;

	float mRotateSpeed;

	/////////////////////////////////////
	// Actor components.

	jkCamera* m_pCamera;
};

#endif // !JKACTOR_H_