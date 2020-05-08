#pragma once
#include "Mesh.h"

class jkSkeletalMesh :
	public jkMesh
{
	friend class jkContent;
	friend class jkResourceManager;
	friend class jkAnimator;

public:

	jkSkeletalMesh() : jkMesh() {}

protected:

	/////////////////////////////////
	// Skeletal animations.

	std::unordered_map<std::string, int> mBoneIDMap;//From bone name to id(in mBoneOffsetMatrices).
	std::vector<MAT4> mBoneOffsetMatrices;//From model space position to bone space position.
	std::vector<MAT4> mBoneMatrices;//Current bone matrices (with animation adjust).

	std::vector<Animation> mAnimations;
	//TODO : a animation name.

};

