#pragma once
#ifndef JKBACKENDRENDERER_H_
#define JKBACKENDRENDERER_H_

#include"Types.h"
#include"Mesh.h"
#include "Light.h"
#include<unordered_map>

class jkBackendRenderer
{
public:

	jkBackendRenderer() : mSkybox(nullptr), mDirLight(nullptr), mSpotLight(nullptr) { mPointLights.clear(); }

	virtual ~jkBackendRenderer() {
		delete mDirLight; delete mSpotLight; 
		for (auto pl : mPointLights)
		{
			delete pl;
		}
		delete mSkybox;
	}

	virtual void Clear() = 0;

	virtual inline void SetClearColor(COLOR3 clearColor = { 0.7f, 0.7f, 0.7f }) { mClearColor = clearColor; };

	virtual void StartUp() = 0;

	inline void LoadMesh(jkMesh* mesh) 
	{ 
		auto data = mProcessMesh(mesh);
		mRenderDatas.push_back(data);
		mRenderDataMap.insert(std::make_pair(mesh, data));
	};

	inline void LoadInstanceData(jkMesh* instanceMesh, std::vector<MAT4>& modelMatrices)
	{
		auto data = mProcessInstanceData(instanceMesh, modelMatrices);
		mInstanceRenderDatas.push_back(data);
		mInstanceRenderDataMap.insert(std::make_pair(instanceMesh, data));
	}

	inline void SetUpSkybox(std::vector<unsigned char*>& faces, const ImageFormat& textureFormat) 
	{ mSkybox = mProcessCubeMap(faces, textureFormat); };

	inline void LoadSurroundingData(std::vector<VEC3>& positions, Texture* surrounding_tex)
	{
		mSurroundingRenderDatas.push_back(mProcessSurroundingData(positions, surrounding_tex));
	}
	// TODO : redesign renderData with different 'type'.

	///////////////////////////////////////
	// Dynamic terrain rendering.

	virtual void ChangeDataIndices(jkMesh* p_mesh, std::vector<UINT>& indices) = 0;

	virtual void ChangeInstances(jkMesh* p_ins_mesh, std::vector<MAT4>& modelMatrices) = 0;

	virtual void ChangeSurrounding(std::vector<VEC3>& positions) = 0;

	virtual void StartRender() = 0; // Render all loaded render data.

	virtual void StartDeferredShading() = 0; // DeferredShading.

	virtual void RenderSkybox() = 0;

	inline MAT4& GetModelMatrix() { return mModelMatrix; };
	inline MAT4& GetViewMatrix() { return mViewMatrix; };
	inline MAT4& GetProjMatrix() { return mProjMatrix; };

	inline void SetViewPos(const VEC3& pos) { mViewPos = pos; }

	inline void SetUpLight(DirectionLight* light) { if (mDirLight) delete mDirLight; mDirLight = light; }

	inline void SetUpLight(PointLight* light) { if(mPointLights.size()<MAXLIGHTSNUM) mPointLights.push_back(light); }

	inline void SetUpLight(SpotLight* light) { if (mSpotLight) delete mSpotLight; mSpotLight = light; }

protected:

	COLOR3 mClearColor;

	virtual RenderData* mProcessMesh(jkMesh* mesh) = 0;

	virtual InstanceRenderData* mProcessInstanceData(jkMesh* instanceMesh, std::vector<MAT4>& modelMatrices) = 0;

	virtual CubeMapData* mProcessCubeMap(std::vector<unsigned char*>& faces, const ImageFormat& textureFormat) = 0;

	CubeMapData* mSkybox;

	virtual SurroundingRenderData* mProcessSurroundingData(std::vector<VEC3>& positions, Texture* tex) = 0;

	VEC3 mViewPos;

	MAT4 mModelMatrix;
	MAT4 mViewMatrix;
	MAT4 mProjMatrix;

	std::vector<RenderData*> mRenderDatas;
	std::unordered_map<jkMesh*, RenderData*> mRenderDataMap;
	std::unordered_map<jkMesh*, InstanceRenderData*> mInstanceRenderDataMap;
	std::vector<InstanceRenderData*> mInstanceRenderDatas;
	std::vector<SurroundingRenderData*> mSurroundingRenderDatas;

	DirectionLight* mDirLight;
	std::vector<PointLight*> mPointLights;
	SpotLight* mSpotLight;
};

#endif // !JKBACKENDRENDERER_H_



