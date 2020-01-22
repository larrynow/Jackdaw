#pragma once
#ifndef JKTIMERWIN32
#define JKTIMERWIN32

#include"Timer.h"
#include"Singleton.h"
#include"windows.h"

class jkTimerWin32 : public jkTimer, public jkSingleton<jkTimerWin32>
{
	inline void ReadTimerFrequency() override
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&mCyclesPerSecond);
		mSecondsPerCycle = (1.f) / mCyclesPerSecond;
	}

	inline void QueryTimerTicks() override
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&mCurrentCycle);
	}
};

#endif // !TIMERWIN32
