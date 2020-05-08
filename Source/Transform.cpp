#include "Transform.h"

void jkTransform::mMakeDirections()
{
	// From eulerAngle to directions.
	// Default(x=y=z=0) directions : Front: x - positive, Right : z - positive.

	mFront = VEC3(cos(GetRadian(mEulerAngle.x)) * cos(GetRadian(mEulerAngle.y)),
		sin(GetRadian(mEulerAngle.x)),
		cos(GetRadian(mEulerAngle.x)) * sin(GetRadian(mEulerAngle.y))).Normalize();
	mRight = mFront.CrossProduct({ 0.f, 1.0f, 0.f }).Normalize();
	mUp = mRight.CrossProduct(mFront).Normalize();
}
