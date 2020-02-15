#include "Actor.h"
#include "Jackdaw.h"

void jkActor::AddMovement(const VEC3& direction, float movement)
{
	// Move actor position and its camera.
	mPosition = mPosition + (direction * movement);
	m_pCamera->MoveCamera(direction, movement);
}

void jkActor::Move(const VEC3& direction, float speed)
{
	AddMovement(direction, speed * pTimer->GetDeltaTime());
}
