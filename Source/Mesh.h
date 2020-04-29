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
	friend class jkFrontendRenderer;
	friend class jkBackendRenderer;
	friend class jkBackendRendererGL;
	friend class jkResourceManager;
	friend class jkGeometry;
	friend class jkTerrain::jkTerrainManager;

public:

	//Default constructor.
	jkMesh() : m_bRenderable(true), m_bLighting(true), m_bShinning(false), m_bStatic(true),
		mPosition(0.f, 0.f, 0.f), 
		mScaleX(1.f), mScaleY(1.f), mScaleZ(1.f),
		mRotationPitch(0.f), mRotationYaw(0.f), mRotationRoll(0.f),
		m_pShader(nullptr) {};

	//Constructor with position.
	jkMesh(const VEC3& _pos) : jkMesh() { mPosition = _pos; mFunction_UpdateTranslateMatrix(); };

	//Constructor with position and transform.
	jkMesh(const VEC3& _pos, const VEC3& _scales, const VEC3& _rotations) :
		m_bRenderable(true), m_bLighting(true), m_bShinning(false), m_bStatic(true),
		mPosition(_pos), 
		mScaleX(_scales.x), mScaleY(_scales.y), mScaleZ(_scales.z),
		mRotationPitch(_rotations.x), mRotationYaw(_rotations.y), mRotationRoll(_rotations.z),
		m_pShader(nullptr)
	{
		mFunction_UpdateTranslateMatrix();
		mFunction_UpdateScaleMatrix();
		mFunction_UpdateRotateMatrix();
	};

	~jkMesh() { }

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

	/*
	* Tranform.
	*/

	/////////////////////////////////////////////////////////////////////
	// Move and get position.

	inline void MoveTo(const VEC3& _pos) { mPosition = _pos; mFunction_UpdateTranslateMatrix(); };
	inline VEC3 GetPosition() const { return mPosition; };

	/////////////////////////////////////////////////////////////////////
	// Adjust rotations(loacl space), renew rotate matrix.

	inline void SetRotation(float _pitch, float _yaw, float _roll) { mRotationPitch = _pitch; mRotationYaw = _yaw; mRotationRoll = _roll; mFunction_UpdateRotateMatrix(); };
	inline jkMesh* RotatePitch(float _angle) { mRotationPitch += _angle; mFunction_UpdateRotateMatrix(); return this; };
	inline jkMesh* RotateYaw(float _angle) { mRotationYaw += _angle; mFunction_UpdateRotateMatrix(); return this; };
	inline jkMesh* RotateRoll(float _angle) { mRotationRoll += _angle; mFunction_UpdateRotateMatrix(); return this; };

	////////////////////////////////////////////////////////////////////
	// Adjust scales, renew scale matrix.

	inline void SetScale(float _sX, float _sY, float _sZ) { mScaleX = _sX; mScaleY = _sY; mScaleZ = _sZ; mFunction_UpdateScaleMatrix(); };
	inline void SetScale(float _scale) { mScaleX = mScaleY = mScaleZ = _scale; mFunction_UpdateScaleMatrix(); };
	inline jkMesh* ScaleUpX(float _value) { mScaleX *= _value; mFunction_UpdateScaleMatrix(); return this; };
	inline jkMesh* ScaleUpY(float _value) { mScaleY *= _value; mFunction_UpdateScaleMatrix(); return this; };
	inline jkMesh* ScaleUpZ(float _value) { mScaleZ *= _value; mFunction_UpdateScaleMatrix(); return this; };
	inline jkMesh* ScaleUpXYZ(float _value) { mScaleX *= _value; mScaleY *= _value; mScaleZ *= _value; 
		mFunction_UpdateScaleMatrix(); return this;
	};

	///////////////////////////////////////////////////////////////////
	// Use mRotateMatrix, mScaleMatrix, mTranslateMatrix to get mWorldMatrix.

	MAT4 GetWorldMatrx() { mFunction_UpdateWorldMatrix(); return mWorldMatrix; };// Update when world Matrix is needed. Can be improved by NOT update when transform is not changed.

private:

	bool m_bRenderable;//Render it or not.
	bool m_bLighting;//Calculate lighting or not.
	bool m_bShinning;//Calculate bloom for it or not.
	bool m_bStatic;//Mesh static or not.
	glShader* m_pShader;

	//////////////////////////////////////////////////////////////////
	// Mesh transforms.

	VEC3 mPosition;
	float mScaleX, mScaleY, mScaleZ;
	float mRotationPitch, mRotationYaw, mRotationRoll;// Angle.

	/////////////////////////////////////////////////////////////////
	// World matrix.

	MAT4 mRotateMatrix;
	MAT4 mScaleTranslateMatrix;// For both translate and scale.
	MAT4 mWorldMatrix;

	////////////////////////////////////////////////////////////////
	// Matrix helper functions.

	void mFunction_UpdateRotateMatrix();
	void mFunction_UpdateScaleMatrix();
	void mFunction_UpdateTranslateMatrix();
	void mFunction_UpdateWorldMatrix();

	////////////////////////////////////////////////////////////////
	// Buffers and textures.

	std::vector<Vertex> mVertexBuffer;
	std::vector<UINT> mIndexBuffer;
	//std::vector<Texture*> mTextures;

	Material mMaterial;

	

};


#endif // !JKMESH_H_