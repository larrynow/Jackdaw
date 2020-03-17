#include "WinInputManager.h"
#include "Content.h"
#include "FrontendRenderer.h"

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

void jkWinInputManager::ResetMouseInput()
{
	//Reset mouse to Center of window.
	MouseMove = false;
	auto pos = jkContent::GetInstance().m_pFrontendRenderer->GetScreenPosition(
		{ jkContent::GetInstance().mWidth / 2, jkContent::GetInstance().mHeight / 2 });
	
	SetCursorPos(pos.Width, pos.Height);
	//Rest Last mouse to center for calculating x, y values.
	LastMousePosX = jkContent::GetInstance().mWidth / 2;
	LastMousePosY = jkContent::GetInstance().mHeight / 2;
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
