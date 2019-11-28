#pragma once
#ifndef JKBACKENDRENDERER_H_
#define JKBACKENDRENDERER_H_

#include"Types.h"
#include"Mesh.h"

class jkBackendRenderer
{
public:

	virtual void Clear() = 0;

	virtual inline void SetClearColor(COLOR3 clearColor = { 0.7f, 0.7f, 0.7f }) { mClearColor = clearColor; };

	virtual void StartUp() = 0;

	//virtual void Render(RenderData& data) = 0;

	virtual void DrawMesh(jkMesh* mesh) = 0;

	inline MAT4& GetModelMatrix() { return mModelMatrix; };
	inline MAT4& GetViewMatrix() { return mViewMatrix; };
	inline MAT4& GetProjMatrix() { return mProjMatrix; };

protected:

	COLOR3 mClearColor;

	MAT4 mModelMatrix;
	MAT4 mViewMatrix;
	MAT4 mProjMatrix;
};

#endif // !JKBACKENDRENDERER_H_



