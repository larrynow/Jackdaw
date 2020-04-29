#include "Actor.h"
#include "Jackdaw.h"

void jkActor::AddMovement(const VEC3& direction, float movement)
{
	// Move actor position and its camera.
	mTransform.Translate(direction, movement);
	//mPosition = mPosition + (direction * movement);
	m_pCamera->MoveCamera(direction, movement);
}

void jkActor::Move(const VEC3& direction, float speed)
{
	AddMovement(direction, speed * pTimer->GetDeltaTime());
}

void jkActor::RotatePitch(float value)
{
	mTransform.RotatePitch(value);
	//m_pModel->RotatePitch(value); 
	m_pCamera->RotatePitch(value * mMoveSpeed * pTimer->GetDeltaTime());
}

void jkActor::RotateYaw(float value)
{
	mTransform.RotateYaw(value);
	//m_pModel->RotateYaw(value); 
	m_pCamera->RotateYaw(value * mMoveSpeed * pTimer->GetDeltaTime());
}

void jkActor::RotateRoll(float value)
{
	mTransform.RotateRoll(value);
	//m_pModel->RotateRoll(value); 
	m_pCamera->RotateRoll(value * mMoveSpeed * pTimer->GetDeltaTime());
}
