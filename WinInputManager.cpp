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

int jkWinInputManager::GetInputId(jkInput)
{
	return -1;
}
