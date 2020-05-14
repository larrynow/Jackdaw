#include "Animator.h"
#include "Model.h"
#include "Content.h"

void jkAnimator::RecurUpdateBoneMatrix(jkModel& model,
	const double tick, const AnimationNode* curNode, const MAT4& parentTransform)
{
	MAT4 current_transform;
	if (auto kf_anim = curNode->pAnimationKeys)// With keyframe animation.
	{
		MAT4 translatMatrix = InterpolateTranslationMatrix(tick, kf_anim->positionKeys);
		MAT4 rotationMatrix = InterpolateRotationMatrix(tick, kf_anim->rotationKeys);
		MAT4 scalingMatrix = InterpolateScalingMatrix(tick, kf_anim->scalingKeys);
		current_transform =
			translatMatrix * rotationMatrix * scalingMatrix;
	}
	else
	{
		current_transform = curNode->transformWithParent;
	}

	//If is a bone.
	if (model.mBoneIDMap.count(curNode->nodeName))
	{
		auto id = model.mBoneIDMap.at(curNode->nodeName);
		model.mBoneMatrices[id] = parentTransform * current_transform
			* model.mBoneOffsetMatrices[id];
		//MakeRotationMatrix_Euler(model.mBoneMatrices[id], GetRadian({static_cast<float>(tick), 0.f, 0.f}));
	}

	for (auto pChild : curNode->childNodeAnims)
	{
		RecurUpdateBoneMatrix(model, tick, pChild,
			parentTransform * current_transform);
	}

}

MAT4 jkAnimator::InterpolateTranslationMatrix(const double tick,
	const std::vector<KeyInfo<VEC3>>& positionKeys)
{
	//if (tick == 0) return MAT4();

	MAT4 ret;
	auto translate = InterpolateKeyInfo(tick, positionKeys);
	MakeTranslateMatrix(ret, translate);

	return ret;
}

MAT4 jkAnimator::InterpolateRotationMatrix(const double tick,
	const std::vector<KeyInfo<Quaternion>>& rotationKeys)
{
	//if (tick == 0) return MAT4();

	MAT4 ret;
	auto rotation = InterpolateKeyInfo(tick, rotationKeys);
	MakeRotationMatrix_Quaternion(ret, rotation);

	return ret;
}

MAT4 jkAnimator::InterpolateScalingMatrix(const double tick,
	const std::vector<KeyInfo<VEC3>>& scalingKeys)
{
	//if (tick == 0) return MAT4();

	MAT4 ret;
	auto scaleVec = InterpolateKeyInfo(tick, scalingKeys);
	MakeScaleMatrix(ret, scaleVec);

	return ret;
}

void jkAnimator::Play(std::string anim_name)
{
	static double tick = 0.0;
	std::cout << "play at tick : "<< tick << std::endl;
	if (m_rAnimationNameMap.count(anim_name))
	{
		auto& anim = m_rModel.mAnimations[m_rModel.mAnimationNameMap.at(anim_name)];
		UpdateBoneMatrices(m_rModel, anim, tick);

		tick += anim.tickPerSec * jkContent::GetInstance().m_pTimer->GetDeltaTime();
		if (tick > anim.maxTime) tick = 0.0;// In circulation way.
	}
}
