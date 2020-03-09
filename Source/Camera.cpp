#include "Camera.h"

void jkCamera::MoveCamera(VEC3 direction, float distance)
{
	mCameraPos += (distance * direction);
}

void jkCamera::LookAt(const VEC3& lookAtPoint)
{
	// When looking point changes, update rotations first, then update direction by those new rotations in mFunction_MakeDirection().
	if ((lookAtPoint - mCameraPos).Length() < 0.001f) // Too near.
		return;
	if ((lookAtPoint - mCameraPos).Normalize() == mCameraFront)// Not changing.
		return;
	mCameraFront = (lookAtPoint - mCameraPos).Normalize();
	mFunction_MakeDirections();
	mFunction_UpdateRotations();
};

void jkCamera::MakeViewMatrix(MAT4& mat) const
{
	MakeLookAtMatrix(mat, mCameraPos, mCameraFront, {0.f, 1.f, 0.f});
	//// First a shift matrix.
	//MAT4 shiftMat;
	//shiftMat.SetRow(0, { 1, 0, 0, -mCameraPos.x });
	//shiftMat.SetRow(1, { 0, 1, 0, -mCameraPos.y });
	//shiftMat.SetRow(2, { 0, 0, 1, -mCameraPos.z });
	//shiftMat.SetRow(3, { 0, 0, 0, 1 });

	//// Then made a vector space tans matrix.
	//VEC3 rightVector = mCameraFront.CrossProduct({ 0.f, 1.0f, 0.f });
	//VEC3 upVector = rightVector.CrossProduct(mCameraFront);
	//MAT4 transMat;
	//transMat.SetRow(0, { rightVector.x, rightVector.y, rightVector.z, 0 });
	//transMat.SetRow(1, { upVector.x, upVector.y, upVector.z, 0 });
	//transMat.SetRow(2, { -mCameraFront.x, -mCameraFront.y, -mCameraFront.z, 0 });
	//transMat.SetRow(3, { 0, 0, 0, 1 });

	//mat = transMat * shiftMat;
}

void jkCamera::mFunction_MakeDirections()
{
	// From rotations to directions.
	mCameraFront = VEC3(cos(GetRadian(pitch)) * cos(GetRadian(yaw)), sin(GetRadian(pitch)), cos(GetRadian(pitch)) * sin(GetRadian(yaw))).Normalize();
	mCameraRight = mCameraFront.CrossProduct({ 0.f, 1.0f, 0.f }).Normalize();
	mCameraUp = mCameraFront.CrossProduct(mCameraRight).Normalize();

}

void jkCamera::mFunction_UpdateRotations()
{
	//////////////////////////////////////
	// In left hand coordinate.

	// Yaw:
	yaw = atan(mCameraFront.x / mCameraFront.z);

	// Pitch:
	pitch = atan(-mCameraFront.y / sqrt(mCameraFront.x * mCameraFront.x + mCameraFront.z * mCameraFront.z));

}
