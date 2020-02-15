#include "FrontendRendererWin32.h"
#include"InputManager.h"
#include<assert.h>
#include<glad/glad.h>
#include<wingdi.h>

//#pragma comment (lib, "opengl32.lib")
#include "imm.h"
#pragma comment(lib, "imm32.lib")

LRESULT jkFrontendRendererWin32::mEventsHandler(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) 
{
	switch (msg) 
	{
	case WM_CLOSE: 
		jkInputManager::ExitStatus = 1;
		break;
	case WM_KEYDOWN:
		jkInputManager::KeyStatus[wParam & 511] = 1;
		break;
	case WM_KEYUP:
		jkInputManager::KeyStatus[wParam & 511] = 0;
		break;
	default: 
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

void jkFrontendRendererWin32::Init(UINT bufferWidth, UINT bufferHeight, jkBackendDevice backendDevice)
{
	{
		////////////////////////////////
		// Initialize window.
		
		// Create window class.
		WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)this->mEventsHandler, 0, 0, 0,
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

		/////////////////////////////////////////////////////
		// Create OPENGL context.

		if (backendDevice == jkBackendDevice::OPENGL)
		{
			assert(mCreateGLContext(m_hWindowHandle));
			assert(gladLoadGL());
		}
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

		if (backendDevice == jkBackendDevice::OPENGL)
			glViewport(0, 0, mWindowWidth, mWindowHeight);// TODO : Resize.

		ImmDisableIME(0);// Disable IME.

	}
}

void jkFrontendRendererWin32::Display()
{
	auto hdc = GetDC(m_hWindowHandle);
	SwapBuffers(hdc);
	ReleaseDC(m_hWindowHandle, hdc);
}

bool jkFrontendRendererWin32::mCreateGLContext(HWND hWnd)
{
	// Get window DC.
	HDC hDC = GetDC(hWnd);
	if (!hDC) return false;

	// Set window pixel format.
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// 上述格式描述符的大小
		1,								// 版本号
		PFD_DRAW_TO_WINDOW |			// 格式支持窗口
		PFD_SUPPORT_OPENGL |			// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,				// 必须支持双缓冲
		PFD_TYPE_RGBA,					// 申请 RGBA 格式
		32,							// 选定色彩深度
		0, 0, 0, 0, 0, 0,				// 忽略的色彩位
		0,								// 无Alpha缓存
		0,								// 忽略Shift Bit
		0,								// 无累加缓存
		0, 0, 0, 0,						// 忽略聚集位
		16,								// 16位 Z-缓存 (深度缓存)
		0,								// 无蒙板缓存
		0,								// 无辅助缓存
		PFD_MAIN_PLANE,					// 主绘图层
		0,								// Reserved
		0, 0, 0							// 忽略层遮罩
	};

	// Check if pixel format is matched with window DC.
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!pixelFormat) return false;

	// Set pixel format.
	if (!SetPixelFormat(hDC, pixelFormat, &pfd)) return false;

	// Create render context.
	m_hGLRC = wglCreateContext(hDC);
	if (!m_hGLRC) return false;

	// Connect.
	if (!wglMakeCurrent(hDC, m_hGLRC)) return false;

	ReleaseDC(hWnd, hDC);

	return true;
}

void jkFrontendRendererWin32::mDestroyGLContext()
{
	if (m_hGLRC)
	{
		// Realease device context hRC.
		if (wglMakeCurrent(NULL, NULL))
		{
			// Delete device context hRC.
			if (wglDeleteContext(m_hGLRC))
			{
				m_hGLRC = NULL;
			}
		}
	}
}