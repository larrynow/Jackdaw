#pragma once
#ifndef JKCLOCKWIN32
#define JKCLOCKWIN32

#include"Clock.h"
#include"Singleton.h"
#include"windows.h"

class jkClockWin32 : public jkClock, public jkSingleton<jkClockWin32>
{
	inline void ReadTimerFrequency() override
	{
		// this LARGE_INTEGER is positive,can be transformed to UINT64 safely.
		QueryPerformanceFrequency((LARGE_INTEGER*)&mCyclesPerSecond);
		mSecondsPerCycle = (1.f) / mCyclesPerSecond;
	}

	inline void QueryTimerTicks() override
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&mCurrentCycle);
	}
};

#endif // !JKCLOCKWIN32
