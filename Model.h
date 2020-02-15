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

private:

	//VEC3 mPosition;

	Transform mTransform;

	std::vector<jkMesh*> mMeshes;
};

#endif // !JKMODEL