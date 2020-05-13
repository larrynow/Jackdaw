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
		m_pCamera(new jkCamera(cameraPosition)), m_bIsPitchRestricted(true),
		mMoveSpeed(moveSpeed), mRotateSpeed(rotateSpeed),
		mMoveDirection()
	{
		//Also adjust camera direction to entity front direction.
		m_pCamera->LookAt(m_pCamera->GetTransform().GetPosition()+mTransform.GetFront());
	}

	jkActor() : jkActor({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }) {};// Default actor camera position is actor pos.
	~jkActor() { delete m_pCamera; };

	/////////////////////////////////////////
	// Move both actor and its camera.

	virtual void AddMovement(const VEC3& direction, float movement);

	// Move with current direction and speed.
	virtual inline void UpdateMovement(const double delta_time) { AddMovement(mMoveDirection, mMoveSpeed*delta_time); }
	
	/*inline virtual void MoveForward(float scaling=1.f) { 
		Move(m_pCamera->GetTransform().GetFront(), scaling*mMoveSpeed); 
	}
	inline virtual void MoveBack(float scaling=1.f) { 
		Move(-m_pCamera->GetTransform().GetFront(), scaling*mMoveSpeed); 
	}
	inline virtual void MoveLeft(float scaling=1.f) { 
		Move(-m_pCamera->GetTransform().GetRight(), scaling*mMoveSpeed); 
	}
	inline virtual void MoveRight(float scaling=1.f) { 
		Move(m_pCamera->GetTransform().GetRight(), scaling*mMoveSpeed); 
	}
	inline virtual void MoveUp(float scaling=1.f) { 
		Move(m_pCamera->GetTransform().GetUp(), scaling*mMoveSpeed); 
	}
	inline virtual void MoveDown(float scaling=1.f) { 
		Move(-m_pCamera->GetTransform().GetUp(), scaling*mMoveSpeed); 
	}*/

	virtual void RotatePitch(float scaling = 1.f);
	virtual void RotateYaw(float scaling = 1.f);
	virtual void RotateRoll(float scaling = 1.f);

	inline void BindCamera(jkCamera* pCamera)
	{
		if ((!pCamera) || pCamera == m_pCamera)
			return;
		delete m_pCamera;
		m_pCamera = pCamera;
	}
	inline jkCamera* GetCamera() { return m_pCamera; };

	inline virtual void ZoomUp(float value) { m_pCamera->SetFOV(m_pCamera->GetFOV()*value); }

	inline VEC3& GetMovingDirection() { return mMoveDirection; }

	inline void ChangeMovingDirection(const VEC3& new_dir) { mMoveDirection = new_dir; }

	virtual void Update(const double delta_time) {};

private:

	/////////////////////////////////////
	// Actor status.

	float mMoveSpeed;
	VEC3 mMoveDirection;

	float mRotateSpeed;

	bool m_bIsPitchRestricted;// Restrict pitch range to -89.f to 89.f or not.

	inline float mFunction_ClipPitch(float _val) { 
		if (m_bIsPitchRestricted) return Clamp(_val, -89.0f, 89.0f);
		else { 
			if (_val > 180.f) return _val - 360.f; 
			else if (_val < -180.f) return 360.f + _val; 
			else return _val; }; 
	};

	/////////////////////////////////////
	// Actor components.

protected:
	jkCamera* m_pCamera;
};

#endif // !JKACTOR_H_