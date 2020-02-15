#pragma once
#ifndef JKALGORITHM
#define JKALGORITHM
#include"Types.h"

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
			srand((unsigned)time(NULL));
			k = rand() % (i + 1);
			res[i] = res[k];
			res[k] = arr[i];
		}
	}

	template<typename T>
	void RandomSelect(const std::vector<T>& arr, std::vector<T>& res, const int selectNum)
	{
		InsideOutShuffle(arr, res);
		res(res.begin(), res.begin() + select_num);
	}
}

#endif // !JKALGORITHM
