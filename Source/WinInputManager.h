#pragma once
#ifndef JKWININPUTMANAGER_H_
#define JKWININPUTMANAGER_H_

#include"InputManager.h"
#include<Windows.h>

class jkWinInputManager : public jkInputManager
{
public:

	jkWinInputManager() {};

	void Listen(void) override;

	virtual void ResetMouseInput() override;

	inline virtual void SetMousePos(Rect<UINT> pos) override
	{
		SetCursorPos(pos.Width, pos.Height);
	}

	int MapKey(jkInput input) override;

};

#endif // !JKWININPUTMANAGER_H_