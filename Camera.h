#pragma once
#ifndef JKCAMERA_H_
#define JKCAMERA_H_

#include"Types.h"

class jkCamera
{
public:
	jkCamera() : mCameraPos(0.f, 0.f, -200.f), pitch(0.f), yaw(0.f), roll(0.f), bIsPitchRestricted(true),
		mCameraFront(0.f, 0.f, 1.0f), mCameraRight(1.f, 0.f, 0.f), mCameraUp(0.f, 1.f, 0.f),
		mFOV(45.f), mNearPlane(0.1f), mFarPlane(1000.f) {};
	jkCamera(const VEC3& _pos) : jkCamera() { mCameraPos = _pos; };// Use default directions.
	jkCamera(const VEC3& _pos, const VEC3& _rotations, const float _FOV, const float _nearPlane, const float _farPlane) :
		mCameraPos(_pos), pitch(_rotations.x), yaw(_rotations.y), roll(_rotations.z), mFOV(_FOV), mNearPlane(_nearPlane), mFarPlane(_farPlane)
	{
		mFunction_MakeDirections();// Change default rotations, need to renew directions.
	};
	virtual ~jkCamera(){};

	//////////////////////////////////////////////////////////////////
	// Functions to change camera positions, don't need to change directions and rotations.

	void MoveCamera(VEC3 direction, float distance);
	inline void MoveTo(VEC3 newPos) { mCameraPos = newPos; };

	// Get current camera position.
	inline VEC3 GetPos() { return mCameraPos; };

	/////////////////////////////////////////////////////////////////
	// Functions to change rotations, also need to renew directions.

	inline void SetPitch(float _pitch) { pitch = _pitch; mFunction_MakeDirections(); };
	inline void SetYaw(float _yaw) { yaw = _yaw; mFunction_MakeDirections(); };
	inline void SetRoll(float _roll) { roll = _roll; mFunction_MakeDirections(); };
	inline void SetRotations(float _pitch, float _yaw, float _roll) { pitch = _pitch; yaw = _yaw; roll = _roll; mFunction_MakeDirections(); };

	inline void RotatePitch(float _add_value) { SetPitch(mFunction_ClipPitch(pitch + _add_value)); };// Need clamp in someway.
	inline void RotateYaw(float _add_value) { SetYaw(mFunction_ClipRotation(pitch + _add_value)); };
	inline void RotateRoll(float _add_value) { SetRoll(mFunction_ClipRotation(pitch + _add_value)); };

	////////////////////////////////////////////////////////////////
	// Get camera direction vectors.

	inline VEC3 GetFront() { return mCameraFront; };
	inline VEC3 GetRight() { return mCameraRight; };
	inline VEC3 GetUp() { return mCameraUp; };

	// New looking point, change cameraFront, Rotations and Directions.
	void LookAt(const VEC3& lookAtPoint);

	// Function to adjust FOV, change viewMatrix.
	inline void SetFOV(float _FOV) { mFOV = _FOV; };

	inline float GetFOV() { return mFOV; };
	inline float GetNearPlane() { return mNearPlane; };
	inline float GetFarPlane() { return mFarPlane; };

	// Get current View Matrix by mCameraFront vector.
	void GetViewMatrix(MAT4& outMat) const;

private:

	VEC3 mCameraPos;

	float yaw, pitch, roll;
	bool bIsPitchRestricted;// Restrict pitch range to -89.f to 89.f or not.

	VEC3 mCameraFront;
	VEC3 mCameraRight;
	VEC3 mCameraUp;

	float mFOV, mMinFOV, mMaxFOV;
	float mNearPlane, mFarPlane;
	float mAspectRatio;

	//////////////////////////////////////////////////////
	// Help functions.

	void mFunction_MakeDirections();
	void mFunction_UpdateRotations();

	inline float mFunction_ClipRotation(float _val) { if (_val > 180.f) return _val - 360.f; else if (_val < -180.f) return 360.f + _val; else return _val; };
	inline float mFunction_ClipPitch(float _val) { if (bIsPitchRestricted) return Clamp(_val, -89.0f, 89.0f); else { if (_val > 180.f) return _val - 360.f; else if (_val < -180.f) return 360.f + _val; else return _val; }; };

};

#endif // !JKCAMERA_H_
