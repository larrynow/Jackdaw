#include"CLockWin32.h"

float jkClock::mSecondsPerCycle = 0.f;
U64 jkClock::mCyclesPerSecond = 0;
jkClockWin32* jkSingleton<jkClockWin32>::m_pInstance = nullptr;