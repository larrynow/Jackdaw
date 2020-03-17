#pragma once
#ifndef FRONTENDRENDERER_H_
#define FRONTENDRENDERER_H_

#include"Types.h"
#include"Devices.h"
#include"Mesh.h"
#include"InputManager.h"

/*
jkFrontRenderer : device renderer, sharing buffers with BackendRenderer.
*/

class jkFrontendRenderer
{
public:

	virtual ~jkFrontendRenderer() {};

	// Init a window.
	virtual void Init(UINT bufferWidth, UINT bufferHeight, jkBackendDevice backendDevice)=0;

	virtual void Display() = 0;

	virtual Rect<UINT> GetScreenPosition(Rect<UINT> window_pos) = 0;

	void PushToBackend(class BackendRenderer*);

protected:

	UINT mBufferWidth;
	UINT mBufferHeight;
	UINT bufferSize;

	unsigned char* m_pFrameBuffer;

	std::vector<RenderData> mRenderDataList;

	jkInputManager* m_pInputManager;

};

#endif // !FRONTENDRENDERER_H_