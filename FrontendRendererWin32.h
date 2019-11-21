#pragma once
#ifndef JKFRONDRENDERERWIN32_H_
#define JKFRONDRENDERERWIN32_H_

#include "FrontendRenderer.h"
#include<Windows.h>
#include"Devices.h"

#define BOX(msg) ::MessageBoxA(0, msg, 0, 0);

class jkFrontendRendererWin32 : public jkFrontendRenderer
{
public:

	void Init(UINT bufferWidth, UINT bufferHeight, jkBackendDevice backendDevice) override;

	void Display() override;

private:

	///////////////////////////////////////////
	// Window and screen.

	HWND m_hWindowHandle;
	UINT mWindowWidth;
	UINT mWindowHeight;

	HGLRC m_hGLRC;

	bool mCreateGLContext(HWND hWnd);
	void mDestroyGLContext();

	// Events handler for created window.
	static LRESULT mEventsHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

#endif // !JKFRONTRENDERERWIN32_H_