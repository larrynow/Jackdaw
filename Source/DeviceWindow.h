#pragma once
#ifndef FRONTENDRENDERER_H_
#define FRONTENDRENDERER_H_

#include"Types.h"
#include"Devices.h"
#include"Mesh.h"

/*
jkWindow : device window setting, sharing buffers with BackendRenderer.
*/

class jkSysInputManager;

class jkDeviceWindow
{
public:

	virtual ~jkDeviceWindow() {};

	// Init a window.
	virtual void Init(UINT bufferWidth, UINT bufferHeight, jkBackendDevice backendDevice)=0;

	virtual void Display() = 0;

	virtual Rect<UINT> GetScreenPosition(Rect<UINT> window_pos) = 0;

protected:

	UINT mBufferWidth;
	UINT mBufferHeight;
	UINT bufferSize;

	unsigned char* m_pFrameBuffer;

	std::vector<RenderData> mRenderDataList;

	jkSysInputManager* m_pInputManager;

};

#endif // !FRONTENDRENDERER_H_