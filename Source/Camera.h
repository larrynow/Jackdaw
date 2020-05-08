#pragma once
#ifndef JKCAMERA_H_
#define JKCAMERA_H_

#include"Types.h"
#include"Transform.h"

class jkCamera
{
public:
	jkCamera(const VEC3& _pos) : mTransform(_pos),
		mFOV(45.f), mMinFOV(30.f), mMaxFOV(90.f),
		mNearPlane(0.1f), mFarPlane(100.f),
		mAspectRatio(1.f){};
	jkCamera() : jkCamera(VEC3(0.f)) {};// Use default directions : Front : x-positive, Right : y-positive.
	
	virtual ~jkCamera(){};

	// New looking point, change cameraFront, Rotations and Directions.
	void LookAt(const VEC3& lookAtPoint);

	// Function to adjust FOV, change projMatrix.
	inline void SetFOV(float _FOV) { mFOV = Clamp(_FOV, mMinFOV, mMaxFOV); };

	void MagnifyFOV() {};
	void ShrinkFOV() {};

	inline void SetNearPlane(float _NearPlane) { mNearPlane = _NearPlane; }
	inline void SetFarPlane(float _FarPlane) { mFarPlane = _FarPlane; }

	inline float GetFOV() { return mFOV; };
	inline float GetNearPlane() { return mNearPlane; };
	inline float GetFarPlane() { return mFarPlane; };

	// Get current View Matrix by mCameraFront vector.
	void MakeViewMatrix(MAT4& mat) const;

	jkTransform& GetTransform() { return mTransform; }

private:

	jkTransform mTransform;

	float mFOV, mMinFOV, mMaxFOV;
	float mNearPlane, mFarPlane;
	float mAspectRatio;

	// Get Euler angle from front vector.
	void mMakeEulerFromFront(const VEC3& nFront);
};

#endif // !JKCAMERA_H_
