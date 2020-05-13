#pragma once

#include"Types.h"

class jkTransform
{

public:

	jkTransform(const VEC3& pos) : mPosition(pos), mScale(1.f), mEulerAngle(0.f)
		//, m_pParentTrans(nullptr)
	{
		mUpdateTranslateMatrix();
		mUpdateScaleMatrix();
		mUpdateRotateMatrix();
		mMakeDirections();

		mUpdateWorldMatrix();
	}
	jkTransform() : jkTransform(VEC3()){}

	/////////////////////////////////////////////////////////////////////
	// Change position.

	inline void MoveTo(const VEC3& _pos) 
	{ 
		if (_pos == mPosition) return;
		mPosition = _pos; 
		mUpdateTranslateMatrix(); 
		mUpdateWorldMatrix();
	}
	inline void Translate(const VEC3& _direction, float _distance)
	{
		MoveTo(mPosition + (_direction * _distance));
	}

	/////////////////////////////////////////////////////////////////////
	// Change rotations(world space), renew rotate matrix.

	inline void SetEulerAngle(const VEC3& eAngle)
	{
		if (eAngle == mEulerAngle) return;
		mEulerAngle.x = mClipEulerAngle(eAngle.x);
		mEulerAngle.y = mClipEulerAngle(eAngle.y);
		mEulerAngle.z = mClipEulerAngle(eAngle.z);

		mUpdateRotateMatrix();
		mMakeDirections();
		mUpdateWorldMatrix();

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
		if (mScale == _scale) return;
		mScale = _scale;
		mUpdateScaleMatrix();
		mUpdateWorldMatrix();

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
	// Link transform.

	/*inline void BindTransform(jkTransform& trans)
	{
		mChildTrans.push_back(&trans);
		trans.m_pParentTrans = this;
	}
	inline void BindTransform(jkTransform* pTrans)
	{
		BindTransform(*pTrans);
	}*/

	////////////////////////////////
	// Public Get functions.

	inline VEC3 GetPosition() const { return mPosition; }
	inline VEC3 GetScale() const { return mScale; }
	inline VEC3 GetEulerAngle() const { return mEulerAngle; }

	inline MAT4 GetWorldMatrix() const
	{
		return mWorldMatrix;
	}

	inline VEC3 GetFront() const { return mFront; }
	inline VEC3 GetRight() const { return mRight; }
	inline VEC3 GetUp() const { return mUp; }

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
	inline void mUpdateWorldMatrix()
	{
		//Update world matrix from updated scaleTranslateMat and rotateMat.
		mWorldMatrix = mScaleTranslateMatrix * mRotateMatrix;
	}

	inline float mClipEulerAngle(float _val)
	{
		if (_val > 180.f) return _val - 360.f;
		else if (_val < -180.f) return 360.f + _val;
		else return _val;
	}

	void mMakeDirections();

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

	VEC3 mFront;
	VEC3 mRight;
	VEC3 mUp;

	/*jkTransform* m_pParentTrans;
	std::vector<jkTransform*> mChildTrans;*/

	//float mScaleX, mScaleY, mScaleZ;
	//float mRotationPitch, mRotationYaw, mRotationRoll;// Angle.

};

