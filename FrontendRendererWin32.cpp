#include "FrontendRendererWin32.h"

static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: 
		//BFContent::screen_exit = 1; break;
	case WM_KEYDOWN:
		//BFContent::screen_keys[wParam & 511] = 1;
		break;
	case WM_KEYUP:
		//BFContent::screen_keys[wParam & 511] = 0;
		break;
	default: 
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void screen_dispatch(void) {
	MSG msg;
	while (1) 
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void jkFrontendRendererWin32::Init(UINT bufferWidth, UINT bufferHeight)
{
	{
		////////////////////////////////
		// Initialize window.

		// Create window class.
		WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
			NULL, NULL, NULL, NULL, "JackdawWindow" };
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);// set arrow.

		// Register window class.
		if (!RegisterClass(&wc)) 
			BOX("Window class register FAILED.");

		// Create handle of window.
		HWND hWindowHandle = CreateWindow("JackdawWindow", "Jackdaw Flag on!",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);

		if (hWindowHandle == NULL)
			BOX("Window handle create FAILED.");

		m_hWindowHandle = hWindowHandle;
		mWindowWidth = bufferWidth;
		mWindowHeight = bufferHeight;

		// Get window DC.
		HDC hDC = GetDC(hWindowHandle);
		m_hScreenDC = CreateCompatibleDC(hDC);
		ReleaseDC(hWindowHandle, hDC);

		// Configure bitmap info.
		BITMAPINFO bi = { {sizeof(BITMAPINFOHEADER), bufferWidth, bufferHeight, 1, 24, BI_RGB,
			(LONG)(bufferWidth * bufferHeight * 3), 0, 0, 0, 0 } };// 24 bits color, 3 channels RGB for 3 byte : /R:8/G:8/B:8.

		// Create bitmap.
		HBITMAP hBmpNew = NULL;
		HBITMAP hBmpOld = NULL;
		LPVOID values;// bitmap's pixel values.

		// Allocate spaces for bmp pixels.
		hBmpNew = CreateDIBSection(m_hScreenDC, &bi, DIB_RGB_COLORS, &values, 0, 0);
		if (hBmpNew == NULL) return;
		m_pFrameBuffer = (unsigned char*)values;
		memset(m_pFrameBuffer, 0, bufferWidth * bufferHeight * 3);

		// Select created bitmap.
		hBmpOld = (HBITMAP)SelectObject(m_hScreenDC, hBmpNew);

		// Adjust window shape, position.
		RECT rect = { 0, 0, (LONG)bufferWidth, (LONG)bufferHeight };
		AdjustWindowRect(&rect, GetWindowLong(hWindowHandle, GWL_STYLE), 0);
		int wx, wy, sx, sy;
		wx = rect.right - rect.left;
		wy = rect.bottom - rect.top;
		sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
		sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
		if (sy < 0) sy = 0;// Too large.
		SetWindowPos(hWindowHandle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
		SetForegroundWindow(hWindowHandle);

		ShowWindow(hWindowHandle, SW_NORMAL);

		ImmDisableIME(0);// Disable IME.

		screen_dispatch();// Deal with message before rendering.

	}
}
