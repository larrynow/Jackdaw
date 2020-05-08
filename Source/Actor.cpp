#include "Actor.h"
#include "Content.h"

void jkActor::Move(const VEC3& direction, float speed)
{
	AddMovement(direction, speed* pTimer->GetDeltaTime());
}

void jkActor::AddMovement(const VEC3& direction, float movement)
{
	mTransform.Translate(direction, movement);
	m_pCamera->GetTransform().Translate(direction, movement);
}

void jkActor::RotatePitch(float value)
{
	mTransform.RotatePitch(value * mMoveSpeed * pTimer->GetDeltaTime());
	m_pCamera->GetTransform().RotatePitch(value * mMoveSpeed * pTimer->GetDeltaTime());
}

void jkActor::RotateYaw(float value)
{
	mTransform.RotateYaw(value * mMoveSpeed * pTimer->GetDeltaTime());
	m_pCamera->GetTransform().RotateYaw(value * mMoveSpeed * pTimer->GetDeltaTime());
}

void jkActor::RotateRoll(float value)
{
	mTransform.RotateRoll(value * mMoveSpeed * pTimer->GetDeltaTime());
	m_pCamera->GetTransform().RotateRoll(value * mMoveSpeed * pTimer->GetDeltaTime());
}
