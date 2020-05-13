#pragma once
#ifndef JKMESH_H_
#define JKMESH_H_

#include"Types.h"

class glShader;
namespace jkTerrain {
	class jkTerrainManager;
}

class jkMesh
{
	friend class jkDeviceWindow;
	friend class jkBackendRenderer;
	friend class jkBackendRendererGL;
	friend class jkResourceManager;
	friend class jkGeometry;
	friend class jkTerrain::jkTerrainManager;

public:

	//Default constructor.
	jkMesh() : m_bRenderable(true), m_bLighting(true), m_bShinning(false), m_bStatic(true),
		//mPosition(0.f, 0.f, 0.f), 
		//mScaleX(1.f), mScaleY(1.f), mScaleZ(1.f),
		//mRotationPitch(0.f), mRotationYaw(0.f), mRotationRoll(0.f),
		m_pShader(nullptr)
	{};

	virtual ~jkMesh() { }

	inline UINT GetVertexCount() const { 
		return mVertexBuffer.size(); 
	};

	inline void BindTexture(Texture* pTexture) 
	{ 
		switch (pTexture->Type)
		{
		case TextureType::Diffuse:
			mMaterial.diffuseMap = pTexture;
			break;
		case TextureType::Specular:
			mMaterial.specularMap = pTexture;
			break;
		case TextureType::Normal:
			mMaterial.normalMap = pTexture;
			CalcTangentSpace();
			break;
		case TextureType::Height:
			mMaterial.heightMap = pTexture;
			break;
		default:
			mMaterial.diffuseMap = pTexture;
			break;
		}
	};

	inline void BindTextures(const std::vector<Texture*>& textures)
	{
		for (auto tex : textures)
		{
			BindTexture(tex);
		}
	}

	inline void BindMaterial(const Material& material) { mMaterial = material; }

	inline Material GetMaterial() const { return mMaterial; }

	inline void BindShader(glShader* pShader) { m_pShader = pShader; }

	inline void DisableLighting() { m_bLighting = false; }
	inline void EnableLighting() { m_bLighting = true; }

	inline void EnableBlooming() { m_bShinning = true; }
	inline void DisableBlooming() { m_bShinning = false; }

	///////////////////////////////////
	// Generate tangent and bitangent.

	void CalcTangentSpace(bool mirror = false);

private:

	bool m_bRenderable;//Render it or not.
	bool m_bLighting;//Calculate lighting or not.
	bool m_bShinning;//Calculate bloom for it or not.
	bool m_bStatic;//Mesh static or not.

	glShader* m_pShader;

	////////////////////////////////////////////////////////////////
	// Buffers and textures.

	std::vector<Vertex> mVertexBuffer;
	std::vector<UINT> mIndexBuffer;
	//std::vector<Texture*> mTextures;

	Material mMaterial;

};


#endif // !JKMESH_H_