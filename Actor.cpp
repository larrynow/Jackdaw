#include "Actor.h"

void jkActor::AddMovement(const VEC3& direction, float movement)
{
	// Move actor position and its camera.
	mPosition = mPosition + (direction * movement);
	m_pCamera->MoveTo(mPosition);
}
