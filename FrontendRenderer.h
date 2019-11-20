#pragma once
#ifndef FRONTENDRENDERER_H_
#define FRONTENDRENDERER_H_

#include"Types.h"
#include"Mesh.h"

/*
jkFrontRenderer : device renderer, sharing buffers with BackendRenderer.
*/

class jkFrontendRenderer
{
public:

	virtual ~jkFrontendRenderer() {};

	virtual void Init(UINT bufferWidth, UINT bufferHeight)=0;

	void RenderMap(jkMap* map) {};

	void RenderMesh(jkMesh* mesh);

protected:

	UINT mBufferWidth;
	UINT mBufferHeight;
	UINT bufferSize;

	unsigned char* m_pFrameBuffer;

};

#endif // !FRONTENDRENDERER_H_