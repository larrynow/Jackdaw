#pragma once

#include"Types.h"

class jkTransform
{

public:

	/*
	* Tranform.
	*/

	/////////////////////////////////////////////////////////////////////
	// Change position.

	inline void MoveTo(const VEC3& _pos) 
	{ 
		mPosition = _pos; 
		mUpdateTranslateMatrix(); 
	}
	inline void Translate(const VEC3& _direction, float _distance)
	{
		MoveTo(mPosition + (_direction * _distance));
	}

	/////////////////////////////////////////////////////////////////////
	// Change rotations(world space), renew rotate matrix.

	inline void SetEulerAngle(const VEC3& eAngle)
	{
		mEulerAngle = eAngle;
		mUpdateRotateMatrix();
	}
	inline void SetEulerAngle(float _pitch, float _yaw, float _roll)
	{ 
		SetEulerAngle({ _pitch, _yaw, _roll });
	}

	inline jkTransform* RotatePitch(float _angle) 
	{ 
		SetEulerAngle(mEulerAngle.x+_angle, mEulerAngle.y, mEulerAngle.z);
		return this;
	}
	inline jkTransform* RotateYaw(float _angle)
	{
		SetEulerAngle(mEulerAngle.x, mEulerAngle.y + _angle, mEulerAngle.z);
		return this;
	}
	inline jkTransform* RotateRoll(float _angle)
	{
		SetEulerAngle(mEulerAngle.x, mEulerAngle.y, mEulerAngle.z + _angle);
		return this;
	}

	////////////////////////////////////////////////////////////////////
	// Change scales, renew scale matrix.

	inline void SetScale(const VEC3& _scale)
	{
		mScale = _scale;
		mUpdateScaleMatrix();
	}
	inline void SetScale(float _sX, float _sY, float _sZ)
	{
		SetScale({ _sX, _sY, _sZ });
	}

	inline jkTransform* ScaleUpX(float _value)
	{
		SetScale(mScale.x * _value, mScale.y, mScale.z);
		return this;
	}
	inline jkTransform* ScaleUpY(float _value)
	{
		SetScale(mScale.x, mScale.y * _value, mScale.z);
		return this;
	}
	inline jkTransform* ScaleUpZ(float _value)
	{
		SetScale(mScale.x, mScale.y, mScale.z * _value);
		return this;
	}
	inline jkTransform* ScaleUpXYZ(float _value)
	{
		SetScale(mScale * _value);
		return this;
	}

	////////////////////////////////
	// Public Get functions.

	inline VEC3 GetPosition() const { return mPosition; }
	inline VEC3 GetScale() const { return mScale; }
	inline VEC3 GetEulerAngle() const { return mEulerAngle; }

	inline MAT4 GetWorldMatrix() const
	{
		return mScaleTranslateMatrix * mRotateMatrix;;
	}

private:

	////////////////////////////////////////////////////////////////
	// Matrix helper functions.

	inline void mUpdateRotateMatrix()
	{
		MakeRotationMatrix_Euler(mRotateMatrix, GetRadian(mEulerAngle));
	}
	inline void mUpdateScaleMatrix()
	{
		MakeScaleMatrix(mScaleTranslateMatrix, mScale);
	}
	inline void mUpdateTranslateMatrix()
	{
		MakeTranslateMatrix(mScaleTranslateMatrix, mPosition);
	}
	//void mUpdateWorldMatrix();

	/////////////////////////////////////////////////////////////////
	// Matrices(update when transform changes immediately).

	MAT4 mRotateMatrix;
	MAT4 mScaleTranslateMatrix;// For both translate and scale.

	MAT4 mWorldMatrix;

	//////////////////////////////
	// Tranform paras.

	VEC3 mPosition;
	VEC3 mScale;
	VEC3 mEulerAngle;//Angle.

	//float mScaleX, mScaleY, mScaleZ;
	//float mRotationPitch, mRotationYaw, mRotationRoll;// Angle.

};

