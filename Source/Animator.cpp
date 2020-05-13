#include "Animator.h"
#include "Model.h"
#include "Content.h"

void jkAnimator::RecurUpdateBoneMatrix(jkModel& model,
	const double tick, const NodeAnimation* curNode, const MAT4& parentTransform)
{
	MAT4 translatMatrix = InterpolateTranslationMatrix(tick, curNode->positionKeys);
	MAT4 rotationMatrix = InterpolateRotationMatrix(tick, curNode->rotationKeys);
	MAT4 scalingMatrix = InterpolateScalingMatrix(tick, curNode->scalingKeys);

	MAT4 current_transform =
		translatMatrix * rotationMatrix * scalingMatrix;

	auto id = model.mBoneIDMap.at(curNode->nodeName);
	model.mBoneMatrices[id] = parentTransform * current_transform
		* model.mBoneOffsetMatrices[id];

	for (auto pChild : curNode->childNodeAnims)
	{
		RecurUpdateBoneMatrix(model, tick, pChild,
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

void jkAnimator::Play(std::string anim_name)
{
	static double tick = 0.0;
	if (m_rAnimationNameMap.count(anim_name))
	{
		auto& anim = m_rModel.mAnimations[m_rModel.mAnimationNameMap[anim_name]];
		UpdateBoneMatrices(m_rModel, anim, tick);

		tick += jkContent::GetInstance().m_pTimer->GetDeltaTime();
		if (tick >= anim.maxTime) tick = 0.0;// In circulation way.
	}
}
