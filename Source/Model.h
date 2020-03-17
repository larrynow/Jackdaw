#pragma once
#ifndef JKMODEL
#define JKMODEL

#include"Mesh.h"

class jkModel
{
	friend class jkContent;
	friend class jkResourceManager;

public:

	jkModel(const VEC3& position) { mTransform.position = position; }
	jkModel() : jkModel({ 0.f, 0.f, 0.f }) {}

	inline void SetPosition(const VEC3& pos) {
		mTransform.position = pos; 
		for (auto pMesh : mMeshes)
			pMesh->MoveTo(pos);
	}

	// Model space rotation.
	inline void Rotate(const VEC3& rotation)
	{
		mTransform.mRotationPitch += rotation.x;
		mTransform.mRotationYaw += rotation.y;
		mTransform.mRotationRoll += rotation.z;

		for (auto pMesh : mMeshes)
		{
			pMesh->RotatePitch(rotation.x);
			pMesh->RotateYaw(rotation.y);
			pMesh->RotateRoll(rotation.z);
		}
	}

	inline void RotatePitch(const float v)
	{
		mTransform.mRotationPitch += v;

		for (auto pMesh : mMeshes)
		{
			pMesh->RotatePitch(v);
		}
	}
	inline void RotateYaw(const float v)
	{
		mTransform.mRotationYaw += v;

		for (auto pMesh : mMeshes)
		{
			pMesh->RotateYaw(v);
		}
	}
	inline void RotateRoll(const float v)
	{
		mTransform.mRotationRoll += v;

		for (auto pMesh : mMeshes)
		{
			pMesh->RotateRoll(v);
		}
	}

private:

	//VEC3 mPosition;

	Transform mTransform;

	std::vector<jkMesh*> mMeshes;
};

#endif // !JKMODEL