#pragma once
#ifndef JKCLOCK
#define JKCLOCK

#include"Types.h"

class jkClock
{
private:

	U64 mTimeCycles;
	float mTimeScale;
	bool m_bIsPaused;
	static float mCyclesPerSecond;

	static inline U64 SecondsToCycles(float timeSeconds)
	{
		return (U64) (timeSeconds * mCyclesPerSecond);
	}

	static inline float CyclesToSeconds(U64 timeCycles)
	{
		return (float)(timeCycles / mCyclesPerSecond);
	}

public:

	explicit jkClock() : m_bIsPaused(false) {}

	void inline Init() { mCyclesPerSecond = 0; }

	void inline Tick() {}

};

#endif // !JKCLOCK
