#pragma once
#ifndef JKTIMER
#define JKTIMER

class jkTimer
{
	friend class jkWheelTimer;

public:
	jkTimer(int rotation, int wheel_slot) :
		mRotation(rotation), mSlotID(wheel_slot) {}


private:
	int mRotation;
	int mSlotID;

	jkTimer* next;
	jkTimer* prev;


};

class jkWheelTimer
{
public:
	jkWheelTimer(float start_time = 0.0f) : mStartTime(start_time), mCurSlot(0)
	{
		for (size_t i = 0; i < WHEELSIZE; i++)
		{
			mSlots[i] = nullptr;
		}
	}

	~jkWheelTimer()
	{
		for (size_t i = 0; i < WHEELSIZE; i++)
		{
			jkTimer* pTimer = mSlots[i];
			while (pTimer)
			{
				mSlots[i] = pTimer->next;
				delete pTimer;
				pTimer = mSlots[i];
			}
		}
	}

private:
	float mStartTime;
	const float TIMEPRECISION = 0.02f;

	static const int WHEELSIZE = 256;
	jkTimer* mSlots[WHEELSIZE];
	int mCurSlot;
};

#endif