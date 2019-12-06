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

	inline void LoadMesh(jkMesh* mesh) { mRenderDatas.push_back(mProcessMesh(mesh)); };

	inline void SetUpSkybox(std::vector<unsigned char*>& faces, const ImageFormat& textureFormat) 
	{ mSkybox = mProcessCubeMap(faces, textureFormat); };

	virtual void StartRender() = 0;

	virtual void RenderSkybox() = 0;

	inline MAT4& GetModelMatrix() { return mModelMatrix; };
	inline MAT4& GetViewMatrix() { return mViewMatrix; };
	inline MAT4& GetProjMatrix() { return mProjMatrix; };

protected:

	COLOR3 mClearColor;

	virtual RenderData* mProcessMesh(jkMesh* mesh) = 0;

	virtual CubeMapData* mProcessCubeMap(std::vector<unsigned char*>& faces, const ImageFormat& textureFormat) = 0;

	CubeMapData* mSkybox;

	MAT4 mModelMatrix;
	MAT4 mViewMatrix;
	MAT4 mProjMatrix;

	std::vector<RenderData*> mRenderDatas;
};

#endif // !JKBACKENDRENDERER_H_



