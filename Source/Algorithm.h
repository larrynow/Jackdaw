#pragma once
#ifndef JKALGORITHM
#define JKALGORITHM
#include"Types.h"
#include<random>

namespace jkAlgorithm
{
	template<typename T>
	void InsideOutShuffle(const std::vector<T>& arr, std::vector<T>& res)
	{
		res.assign(arr.size(), T());
		copy(arr.begin(), arr.end(), res.begin());
		int k;
		for (int i = 0; i < arr.size(); ++i)
		{
			//srand((unsigned)time(NULL));
			k = rand() % (i + 1);
			res[i] = res[k];
			res[k] = arr[i];
		}
	}

	template<typename T>
	void RandomSelect(const std::vector<T>& arr, std::vector<T>& res, const int selectNum)
	{
		InsideOutShuffle(arr, res);
		res= std::vector<T>(res.begin(), res.begin() + selectNum);
	}

	VEC3 RandomSampleFromTriangle(const VEC3& a, const VEC3& b, const VEC3& c)
	{
		VEC3 a_b = b - a;
		VEC3 a_c = c - a;
		VEC3 b_c = c - b;

		//Make a_c as max length line.
		if (a_b.Length() > a_c.Length()) {
			std::swap(a_b, a_c); b_c = -b_c;
		}
		if (b_c.Length() > a_c.Length()) { 
			std::swap(b_c, a_c); a_b = -a_b;
		}

		//Create Rectangle.
		float cos_theta = a_b.DotProduct(a_c) / a_b.Length() / a_c.Length();
		VEC3 upVector = a_b - cos_theta* a_b.Length()* a_c / a_c.Length();
		VEC3 rightVector = a_c;
		float upScale = (float)(rand() % 101) / 100;
		float rightScale = (float)(rand() % 101) / 100;

		VEC3 a_ = { 0.f, 0.f, 0.f };
		VEC3 b_ = { cos_theta * a_b.Length()/a_c.Length(), 1.f, 0.f };
		VEC3 c_ = { 1.f, 0.f, 0.f };

		VEC3 p_ = { rightScale, upScale, 0.f };

		//Judge p out of triangle or not. 
		//If p and a is on differrent side of b_c. Symmetry it with b_c.
		bool p_c_one_side = (a_ - p_).CrossProduct(b_ - p_).z * (a_ - c_).CrossProduct(b_ - c_).x > 0.f;
		bool p_b_one_side = (a_ - p_).CrossProduct(c_ - p_).z * (a_ - b_).CrossProduct(b_ - b_).x > 0.f;
		bool p_a_one_side = (b_ - p_).CrossProduct(c_ - p_).z * (b_ - a_).CrossProduct(c_ - a_).x > 0.f;
		if (!p_c_one_side) p_ = (b_ + a_) - p_;
		else if (!p_b_one_side) p_ = (c_ + a_) - p_;
		else if (!p_a_one_side) p_ = (b_ + c_) - p_;
		
		return a + p_.x * rightVector + p_.y * upVector;
	}

	/*VEC3 RandomSampleFromCircle(const VEC3& o, const VEC3& n, float r)
	{
		
	}*/

	inline float GetArea(const VEC3& a, const VEC3& b, const VEC3& c)
	{
		VEC3 a_b = b - a;
		VEC3 a_c = c - a;

		return a_b.CrossProduct(a_c).Length() / 2;
	}

	inline bool IfPointInTaiangle_SameArea(const VEC3& a, const VEC3& b, const VEC3& c, const VEC3& p)
	{
		VEC3 a_b = b - a;
		VEC3 a_p = p - a;
		VEC3 a_c = c - a;

		VEC3 b_c = c - b;
		VEC3 b_p = p - b;
		VEC3 b_a = a - b;

		VEC3 c_a = a - c;
		VEC3 c_p = p - c;
		VEC3 c_b = b - c;

		if (a_b.CrossProduct(a_p).DotProduct(a_b.CrossProduct(a_c)) < 0)//p and c should in same direction of a_b.
		{
			return false;
		}
		if (b_c.CrossProduct(b_p).DotProduct(b_c.CrossProduct(b_a)) < 0)
		{
			return false;
		}
		if (c_a.CrossProduct(c_p).DotProduct(c_a.CrossProduct(c_b)) < 0)
		{
			return false;
		}

		return true;
	}
}

#endif // !JKALGORITHM
