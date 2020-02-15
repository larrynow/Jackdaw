#pragma once
#ifndef JKCLOCK
#define JKCLOCK

#include"Types.h"

class jkClock
{
protected:

	U64 mTotalCycles;
	U64 mCurrentCycle;

	float mTimeSeconds;
	float mDeltaTime;
	float mTimeScale;

	bool m_bIsPaused;
	static U64 mCyclesPerSecond;
	static float mSecondsPerCycle;

	int mFPS;

	static inline U64 SecondsToCycles(float timeSeconds)
	{
		return (U64) (mCyclesPerSecond * (double)timeSeconds);
	}

	static inline float CyclesToSeconds(U64 timeCycles)
	{
		return (float)(timeCycles * mSecondsPerCycle);
	}

	virtual void ReadTimerFrequency() = 0;
	virtual void QueryTimerTicks() = 0;

public:

	explicit jkClock(float startTimeSeconds = 0.0f) :
		mTotalCycles(SecondsToCycles(startTimeSeconds)), mTimeSeconds(startTimeSeconds),
		mTimeScale(1.f), m_bIsPaused(false) {}

	void inline Init() { ReadTimerFrequency(); QueryTimerTicks(); }

	void inline Tick()
	{
		U64 deltaTick;

		if (m_bIsPaused)
		{
			// Paused.
			deltaTick = 0;
		}
		else
		{
			U64 prevCycles = mCurrentCycle;// Prev tick.
			QueryTimerTicks();// Update current tick.
			deltaTick = mCurrentCycle - prevCycles;
		}
		mFPS = (int)(1.f / (CyclesToSeconds(deltaTick)));

		if (mFPS <= 0)// Too slow, keep fps as 30.
		{
			mDeltaTime = (1.f / 30.f) * mTimeScale;
			mTotalCycles += (U64)(SecondsToCycles(1.f / 30.f));

		}
		else
		{
			mDeltaTime = CyclesToSeconds(deltaTick) * mTimeScale;// Delta time for a frame.
			mTotalCycles += deltaTick;

		}
		
		mTimeSeconds += mDeltaTime;
	}

	//U64 inline GetTimeCycles() { return mTimeCycles; };
	float inline GetTime() { return mTimeSeconds; };
	float inline GetDeltaTime() { return mDeltaTime; };
	int inline GetFPS() { return mFPS; };

	void inline SetTimeScale(float scale) { mTimeScale = scale; };
};

#endif // !JKCLOCK
