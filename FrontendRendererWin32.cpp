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
		sizeof(PIXELFORMATDESCRIPTOR),	// ������ʽ�������Ĵ�С
		1,								// �汾��
		PFD_DRAW_TO_WINDOW |			// ��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL |			// ��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER,				// ����֧��˫����
		PFD_TYPE_RGBA,					// ���� RGBA ��ʽ
		32,							// ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,				// ���Ե�ɫ��λ
		0,								// ��Alpha����
		0,								// ����Shift Bit
		0,								// ���ۼӻ���
		0, 0, 0, 0,						// ���Ծۼ�λ
		16,								// 16λ Z-���� (��Ȼ���)
		0,								// ���ɰ建��
		0,								// �޸�������
		PFD_MAIN_PLANE,					// ����ͼ��
		0,								// Reserved
		0, 0, 0							// ���Բ�����
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