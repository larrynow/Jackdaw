#pragma once
#ifndef JKMESH_H_
#define JKMESH_H_

#include"Types.h"

class jkMesh
{
	friend class jkFrontendRenderer;
	friend class jkBackendRenderer;
	friend class jkBackendRendererGL;
	friend class jkResourceManager;
	friend class jkGeometry;

public:

	jkMesh() : mPosition(0.f, 0.f, 0.f), mScaleX(1.f), mScaleY(1.f), mScaleZ(1.f),
		mRotationPitch(0.f), mRotationYaw(0.f), mRotationRoll(0.f),
		m_pTexture(nullptr)
	{
		//m_pVertexBuffer = new std::vector<Vertex>; m_pIndexBuffer = new std::vector<UINT>;

	};
	jkMesh(const VEC3& _pos) : jkMesh() { mPosition = _pos; mFunction_UpdateTranslateMatrix(); };
	jkMesh(const VEC3& _pos, const VEC3& _scales, const VEC3& _rotations) :
		mPosition(_pos), mScaleX(_scales.x), mScaleY(_scales.y), mScaleZ(_scales.z),
		mRotationPitch(_rotations.x), mRotationYaw(_rotations.y), mRotationRoll(_rotations.z),
		m_pTexture(nullptr)
	{
		//m_pVertexBuffer = new std::vector<Vertex>; m_pIndexBuffer = new std::vector<UINT>;

	};

	inline UINT GetVertexCount() const { 
		//return m_pVertexBuffer->size(); 
		return mVertexBuffer.size(); 
	};

	inline void BindTexture(Texture* _p_texture) { m_pTexture = _p_texture; };

	/////////////////////////////////////////////////////////////////////
	// Move and get position.

	inline void MoveTo(const VEC3& _pos) { mPosition = _pos; mFunction_UpdateTranslateMatrix(); };// Maybe judge collisons first.
	inline VEC3 GetPosition() const { return mPosition; };

	/////////////////////////////////////////////////////////////////////
	// Adjust rotations, renew rotate matrix.

	inline void SetRotation(float _pitch, float _yaw, float _roll) { mRotationPitch = _pitch; mRotationYaw = _yaw; mRotationRoll = _roll; mFunction_UpdateRotateMatrix(); };
	inline void RotateWithX(float _angle) { mRotationPitch += _angle; mFunction_UpdateRotateMatrix(); };
	inline void RotateWithY(float _angle) { mRotationYaw += _angle; mFunction_UpdateRotateMatrix(); };
	inline void RotateWithZ(float _angle) { mRotationRoll += _angle; mFunction_UpdateRotateMatrix(); };

	////////////////////////////////////////////////////////////////////
	// Adjust scales, renew scale matrix.

	inline void SetScale(float _sX, float _sY, float _sZ) { mScaleX = _sX; mScaleY = _sY; mScaleZ = _sZ; mFunction_UpdateScaleMatrix(); };
	inline void SetScale(float _scale) { mScaleX = mScaleY = mScaleZ = _scale; mFunction_UpdateScaleMatrix(); };
	inline void ScaleUpX(float _value) { mScaleX *= _value; mFunction_UpdateScaleMatrix(); };
	inline void ScaleUpY(float _value) { mScaleY *= _value; mFunction_UpdateScaleMatrix(); };
	inline void ScaleUpZ(float _value) { mScaleZ *= _value; mFunction_UpdateScaleMatrix(); };
	inline void ScaleUpXYZ(float _value) { mScaleX *= _value; mScaleY *= _value; mScaleZ *= _value; mFunction_UpdateScaleMatrix(); };

	///////////////////////////////////////////////////////////////////
	// Use mRotateMatrix, mScaleMatrix, mTranslateMatrix to get mWorldMatrix.

	MAT4 GetWorldMatrx() { mFunction_UpdateWorldMatrix(); return mWorldMatrix; };// Update when world Matrix is needed. Can be improved by NOT update when transform is not changed.

private:

	//////////////////////////////////////////////////////////////////
	// Mesh transforms.

	VEC3 mPosition;
	float mScaleX, mScaleY, mScaleZ;
	float mRotationPitch, mRotationYaw, mRotationRoll;// Angle.

	/////////////////////////////////////////////////////////////////
	// World matrix.

	MAT4 mRotateMatrix;
	MAT4 mScaleTranslateMatrix;// For both translate and scale.
	MAT4 mWorldMatrix;

	////////////////////////////////////////////////////////////////
	// Helper functions.

	void mFunction_UpdateRotateMatrix();
	void mFunction_UpdateScaleMatrix();
	void mFunction_UpdateTranslateMatrix();
	void mFunction_UpdateWorldMatrix();

	////////////////////////////////////////////////////////////////
	// Buffers.

	//std::vector<Vertex>* m_pVertexBuffer;// Can be updated in BFResourceManager.
	//std::vector<UINT>* m_pIndexBuffer;

	std::vector<Vertex> mVertexBuffer;
	std::vector<UINT> mIndexBuffer;

	Texture* m_pTexture;
};


#endif // !JKMESH_H_