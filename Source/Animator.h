#pragma once

#include "Types.h"
#include "Model.h"
#include<algorithm>

class jkAnimator
{
public:
	
	// From model get animation.
	jkAnimator(jkModel& model) :
		m_rModel(model),
		m_rAnimations(model.mAnimations), m_rAnimationNameMap(model.mAnimationNameMap) {}

	// Play an animation(change model matrices).
	void Play(std::string anim_name);

	//Update a mesh's bone matrices.
	static inline void UpdateBoneMatrices(jkModel& model, const Animation& animation, const double tick)
	{
		RecurUpdateBoneMatrix(model, tick, animation.pRoot, MAT4());
	}

	static void RecurUpdateBoneMatrix(jkModel& model, const double tick,
		const AnimationNode* curNode, const MAT4& parentTransform);

	static MAT4 InterpolateTranslationMatrix(const double tick,
		const std::vector<KeyInfo<VEC3>>& positionKeys);

	static MAT4 InterpolateRotationMatrix(const double tick,
		const std::vector<KeyInfo<Quaternion>>& rotationKeys);

	static MAT4 InterpolateScalingMatrix(const double tick,
		const std::vector<KeyInfo<VEC3>>& scalingKeys);

	template<typename ValueT>
	static ValueT InterpolateKeyInfo(const double tick,
		const std::vector<KeyInfo<ValueT>>& KeyInfos);

private:

	jkModel& m_rModel;
	std::vector<Animation>& m_rAnimations;
	std::unordered_map<std::string, int>& m_rAnimationNameMap;

};

template<typename ValueT>
inline ValueT jkAnimator::InterpolateKeyInfo(const double tick, const std::vector<KeyInfo<ValueT>>& KeyInfos)
{
	if (KeyInfos.size() == 0) return ValueT();
	if (KeyInfos.size() == 1) return KeyInfos[0].value;

	KeyInfo<ValueT> tickInfo(tick);
	tickInfo = Clamp(tickInfo, *KeyInfos.begin(), *(KeyInfos.end()-1));

	//Find right bound.
	auto right_bound = std::upper_bound(KeyInfos.begin(),
		KeyInfos.end(), tickInfo);//First bigger.
	auto left_bound = right_bound - 1;
	
	if (right_bound == KeyInfos.end()) tickInfo.value = left_bound->value;//Last tick value.
	else if (right_bound == KeyInfos.begin()) tickInfo.value = right_bound->value;//First tick value.
	else
		tickInfo.value = Lerp(left_bound->value, right_bound->value,
			Fraction(left_bound->time, right_bound->time, tickInfo.time));

	return tickInfo.value;
}

template<>
inline Quaternion jkAnimator::InterpolateKeyInfo(const double tick, const std::vector<KeyInfo<Quaternion>>& KeyInfos)
{
	if (KeyInfos.size() == 0) return Quaternion();
	if (KeyInfos.size() == 1) return KeyInfos[0].value;

	KeyInfo<Quaternion> tickInfo(tick);
	tickInfo = Clamp(tickInfo, *KeyInfos.begin(), *(KeyInfos.end() - 1));

	//Find right bound.
	auto right_bound = std::upper_bound(KeyInfos.begin(),
		KeyInfos.end(), tickInfo);//First bigger.
	auto left_bound = right_bound - 1;

	if (right_bound == KeyInfos.end()) tickInfo.value = left_bound->value;//Last tick value.
	else if (right_bound == KeyInfos.begin()) tickInfo.value = right_bound->value;//First tick value.
	else
		tickInfo.value = jkMath::QuaternionSlerp(left_bound->value, right_bound->value,
			Fraction(left_bound->time, right_bound->time, tickInfo.time));

	return tickInfo.value;
}
