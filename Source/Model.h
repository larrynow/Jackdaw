#pragma once
#ifndef JKMODEL
#define JKMODEL

#include"Mesh.h"
#include"Transform.h"
#include<memory>
#include<unordered_map>

// jkModel : resource for rendering, loaded by ResourceManager.

class jkModel
{
	friend class jkBackendRenderer;
	friend class jkContent;
	friend class jkResourceManager;
	friend class jkAnimator;

public:

	jkModel(const std::string& modelPath) : mModelPath(modelPath) {}
	jkModel() : jkModel(std::string()) {}

	jkModel(std::shared_ptr<jkMesh> mesh)
	{
		mMeshes.push_back(mesh);
	}

	virtual ~jkModel() {}

protected:

	std::string mModelPath;

	std::vector<std::shared_ptr<jkMesh>> mMeshes;

	/////////////////////////////////
	// Skeletal animations.

	std::unordered_map<std::string, int> mBoneIDMap;//From bone name to id(in mBoneOffsetMatrices).
	std::vector<MAT4> mBoneOffsetMatrices;//From model space position to bone space position.
	std::vector<MAT4> mBoneMatrices;//Current bone matrices (with animation adjust).

	std::vector<Animation> mAnimations;
	std::unordered_map<std::string, int> mAnimationNameMap; //TODO : a animation name.

};

#endif // !JKMODEL