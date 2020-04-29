#pragma once

#include "Model.h"
#include<algorithm>

class jkAnimator
{
public:
	 
	//Update a model's bone matrices.
	static inline void UpdateBoneMatrices(jkModel* model, const Animation& animation, const double tick)
	{
		RecurUpdateBoneMatrix(model, tick, animation.pRootAnimationNode, MAT4());
	}

	static void RecurUpdateBoneMatrix(jkModel* model, const double tick,
		const NodeAnimation* curNode, const MAT4& parentTransform);

	static MAT4 InterpolateTranslationMatrix(const double tick,
		const std::vector<KeyInfo<VEC3>>& positionKeys);

	static MAT4 InterpolateRotationMatrix(const double tick,
		const std::vector<KeyInfo<VEC4>>& rotationKeys);

	static MAT4 InterpolateScalingMatrix(const double tick,
		const std::vector<KeyInfo<VEC3>>& scalingKeys);

	template<typename ValueT>
	static ValueT InterpolateKeyInfo(const double tick,
		const std::vector<KeyInfo<ValueT>>& KeyInfos);

};

template<typename ValueT>
inline ValueT jkAnimator::InterpolateKeyInfo(const double tick, const std::vector<KeyInfo<ValueT>>& KeyInfos)
{
	KeyInfo<ValueT> tickInfo(tick);
	tickInfo = Clamp(*KeyInfos.begin(), *KeyInfos.end(), tickInfo);

	//Find right bound.
	auto right_bound = std::upper_bound(KeyInfos.begin(),
		KeyInfos.end(), tickInfo);
	auto left_bound = right_bound - 1;

	if (right_bound == KeyInfos.end()) tickInfo.value = left_bound->value;//Last tick value.
	else
		tickInfo.value = Lerp(left_bound->value, right_bound->value,
			Fraction(left_bound->time, right_bound->time, tickInfo.time));

	return tickInfo.value;
}
