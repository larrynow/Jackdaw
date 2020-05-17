#include "Camera.h"

void jkCamera::LookAt(const VEC3& lookAtPoint)
{
	// When looking point changes, update front first, 
	// then update direction by those new rotations in mFunction_MakeDirection().
	auto lookingDir = lookAtPoint - mTransform.GetPosition();
	if (lookingDir.Length() < 0.001f) // Too near.
		return;
	if (lookingDir.Normalize() == mTransform.GetFront())// Not changing.
		return;
	auto newFront = lookingDir.Normalize();
	mMakeEulerFromFront(newFront);
};

void jkCamera::MakeViewMatrix(MAT4& mat) const
{
	MakeLookAtMatrix(mat, mTransform.GetPosition(), mTransform.GetFront(), mTransform.GetUp());
}

void jkCamera::mMakeEulerFromFront(const VEC3& nFront)
{
	//////////////////////////////////////
	// In left hand coordinate.

	// Yaw:
	auto yaw = atan(nFront.x / nFront.z);
	auto pitch = atan(-nFront.y / sqrt(nFront.x * nFront.x + nFront.z * nFront.z));

	mTransform.SetEulerAngle(pitch, yaw, 0.f);
}
