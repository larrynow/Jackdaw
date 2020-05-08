#include "Animator.h"

void jkAnimator::RecurUpdateBoneMatrix(jkSkeletalMesh* mesh,
	const double tick, const NodeAnimation* curNode, const MAT4& parentTransform)
{
	MAT4 translatMatrix = InterpolateTranslationMatrix(tick, curNode->positionKeys);
	MAT4 rotationMatrix = InterpolateRotationMatrix(tick, curNode->rotationKeys);
	MAT4 scalingMatrix = InterpolateScalingMatrix(tick, curNode->scalingKeys);

	MAT4 current_transform =
		translatMatrix * rotationMatrix * scalingMatrix;

	auto id = mesh->mBoneIDMap.at(curNode->nodeName);
	mesh->mBoneMatrices[id] = parentTransform * current_transform 
		* mesh->mBoneOffsetMatrices[id];

	for (auto pChild : curNode->childNodeAnims)
	{
		RecurUpdateBoneMatrix(mesh, tick, pChild,
			parentTransform * current_transform);
	}
}

MAT4 jkAnimator::InterpolateTranslationMatrix(const double tick,
	const std::vector<KeyInfo<VEC3>>& positionKeys)
{
	if (tick == 0) return MAT4();

	//if (tick <= positionKeys.begin()->time)//Before first frame.
	//	return mat_from_vec3(positionKeys.begin()->value);

	//if (tick >= positionKeys.rbegin()->time)//After last frame.
	//	return mat_from_vec3(positionKeys.rbegin()->value);

	MAT4 ret;
	auto translate = InterpolateKeyInfo(tick, positionKeys);
	MakeTranslateMatrix(ret, translate);

	return ret;
}

MAT4 jkAnimator::InterpolateRotationMatrix(const double tick,
	const std::vector<KeyInfo<VEC4>>& rotationKeys)
{
	if (tick == 0) return MAT4();

	MAT4 ret;
	auto rotation = InterpolateKeyInfo(tick, rotationKeys);
	MakeRotationMatrix_Quaternion(ret, rotation);

	return ret;
}

MAT4 jkAnimator::InterpolateScalingMatrix(const double tick,
	const std::vector<KeyInfo<VEC3>>& scalingKeys)
{
	if (tick == 0) return MAT4();

	MAT4 ret;
	auto scaleVec = InterpolateKeyInfo(tick, scalingKeys);
	MakeScaleMatrix(ret, scaleVec);

	return ret;
}
