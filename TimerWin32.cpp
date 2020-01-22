#include"TimerWin32.h"

float jkTimer::mSecondsPerCycle = 0.f;
U64 jkTimer::mCyclesPerSecond = 0;
jkTimerWin32* jkSingleton<jkTimerWin32>::m_pInstance = nullptr;