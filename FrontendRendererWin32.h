#pragma once
#ifndef JKFRONDRENDERERWIN32_H_
#define JKFRONDRENDERERWIN32_H_

#include "FrontendRenderer.h"
#include<Windows.h>

#define BOX(msg) ::MessageBoxA(0, msg, 0, 0);

class jkFrontendRendererWin32 : public jkFrontendRenderer
{
public:

	void Init(UINT bufferWidth, UINT bufferHeight) override;

private:

	///////////////////////////////////////////
	// Window and screen.

	HWND m_hWindowHandle;
	UINT mWindowWidth;
	UINT mWindowHeight;
	HDC m_hScreenDC;// The DC to draw into.
};

#endif // !JKFRONTRENDERERWIN32_H_