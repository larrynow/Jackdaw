#pragma once
#ifndef JKCLOCK
#define JKCLOCK

#include"Types.h"

class jkClock
{
private:

	U64 mStartTimeCycle;
	U64 mTotalTimeCycles;
	U64 mCurrentTimeCycles;
	float mTimeScale;
	bool m_bIsPaused;
	static float mCyclesPerSecond;

	int mFPS;

	static inline U64 SecondsToCycles(float timeSeconds)
	{
		return (U64) (timeSeconds * mCyclesPerSecond);
	}

	static inline float CyclesToSeconds(U64 timeCycles)
	{
		return (float)(timeCycles / mCyclesPerSecond);
	}

	virtual U64 QueryTimeCycles() = 0;

	virtual float QueryTimerFrequency() = 0;

public:

	explicit jkClock() : m_bIsPaused(false), mTimeScale(1.0f) {}

	void inline Init() { mCurrentTimeCycles = mStartTimeCycle = QueryTimeCycles(); mCyclesPerSecond = QueryTimerFrequency(); 
		mTotalTimeCycles = 0; }

	void inline Tick()
	{
		auto prevTimeCycles = mCurrentTimeCycles;
		mCurrentTimeCycles = QueryTimeCycles();

		auto dtCycles = (mCurrentTimeCycles - prevTimeCycles);

		mFPS = mCyclesPerSecond / dtCycles;

		if (!m_bIsPaused)
		{
			mTotalTimeCycles += (dtCycles * mTimeScale);
		}
	}

	void inline SingleStep()
	{
		auto dtCycles = (SecondsToCycles((1.0f / 30.0f) * mTimeScale));
		mCurrentTimeCycles += dtCycles;
		
		mTotalTimeCycles += dtCycles;
	}

	U64 inline GetTime() { return mTotalTimeCycles; }

	int inline GetFPS() { return mFPS; }
};

#endif // !JKCLOCK
