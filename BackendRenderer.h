#pragma once
#ifndef JKBACKENDRENDERER_H_
#define JKBACKENDRENDERER_H_

#include"Types.h"

class jkBackendRenderer
{
public:

	virtual void Clear() = 0;

	virtual inline void SetClearColor(COLOR3 clearColor = { 0.7f, 0.7f, 0.7f }) { mClearColor = clearColor; };

	virtual void StartUp() = 0;

	virtual void Render(RenderData data) = 0;

	inline void SetViewMatrix(MAT4&& view) { mViewMatrix = view; };
	inline void SetProjMatrix(MAT4&& proj) { mProjMatrix = proj; };

protected:

	COLOR3 mClearColor;

	MAT4 mViewMatrix;
	MAT4 mProjMatrix;
};

#endif // !JKBACKENDRENDERER_H_



