#include "WinInputManager.h"

void jkWinInputManager::Listen(void)
{
	MSG msg;
	while (1)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

// Windows inputMap, from VK to BFInput.
int jkWinInputManager::MapKey(jkInput input)
{
	int ret = -1;
	if (input >= jkInput::KEY_A && input <= jkInput::KEY_Z)
	{
		//  Key A to Z is ascii code of 'A' to 'Z'.
		ret = int(input) - int(jkInput::KEY_A) + 65;
	}
	else
	{
		switch (input)
		{
		case(jkInput::KEY_SPACE):
			ret = VK_SPACE;
			break;
		case(jkInput::KEY_SHIFT):
			ret = VK_SHIFT;
			break;
		case(jkInput::KEY_CTRL):
			ret = VK_CONTROL;
			break;
		default:
			break;
		}
	}

	return ret;
}
