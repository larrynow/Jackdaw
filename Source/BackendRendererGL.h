#pragma once
#include "BackendRenderer.h"
#include "GLShader.h"
#include"glad/glad.h"
#include"Mesh.h"
#include"Geometry.h"

struct GLRenderData : public RenderData
{
	GLRenderData() : RenderData(), VAO(0), VBO(0), EBO(0), 
		difffuseTex(0), specularTex(0), normalTex(0),heightTex(0),
		pShader(nullptr){}
	~GLRenderData() 
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		//for (auto tex : TexID)
		{
			//glDeleteTextures(1, &tex);
			//TODO : texture should be delete at the time when no mesh is bind with.
		}
	}

	UINT VAO, VBO, EBO;
	UINT difffuseTex;
	UINT specularTex;
	UINT normalTex;
	UINT heightTex;
	int shininess;
	//std::vector<UINT> TexID;
	glShader* pShader;
};

struct GLInstanceRenderData : public GLRenderData, public InstanceRenderData
{
	UINT InstanceVBO;
	std::vector<MAT4> pModelMatrices;
};

struct GLCubeMapData : public CubeMapData, public GLRenderData{};

struct GLSurroundingRenderData : public SurroundingRenderData
{
	UINT VAO, VBO;
	std::vector<UINT> TexID;
	glShader* pShader;
	int count;
};

class jkBackendRendererGL : public jkBackendRenderer
{
public:

	jkBackendRendererGL(){}

	void SetClearColor(COLOR3 clearColor = { 0.27f, 0.27f, 0.27f });

	void Clear() override;

	void StartUp() override;

	void StartRender() override;

	void StartDeferredShading() override;

	void RenderSkybox() override;

	void ChangeDataIndices(jkMesh* p_mesh, std::vector<UINT>& indices) override;

	void ChangeInstances(jkMesh* p_ins_mesh, std::vector<MAT4>& model_matrices) override;

	void ChangeSurrounding(std::vector<VEC3>& positions) override;

private:

	UINT mLightPosUBO;

	UINT mViewPosUBO;

	UINT mMatrixUBO;//For view and projection matrices.

	UINT mFrameBuffer;

	UINT mHDRFBO;//HDR frame buffer.

	UINT mFloatColorBuffers[2];//Float color buffer texs belongs to HDR FBO.

	UINT mAuxFBOs[3];//Auxiliary FBOs.

	UINT mAuxColorBuffers[3];//Auxiliary color buffers.

	UINT mShadowFBO;//A frame buffer contains a depth texture for shadow.

	UINT mDepthMap;//Shadow map texture.

	UINT mDepthCubemap;//Shadow cubemap texture.

	UINT mQuadVAO;//VAO for post-rendering.

	UINT mQuadVBO;

	//////////////////////////
	// For deferred shading.

	UINT mGBuffer;//A frameBuffer.

	UINT mGPositionBuffer;

	UINT mGNormalBuffer;

	UINT mGAlbedoSpecBuffer;

	UINT mGDepthRBO;

	//////////////////////////
	// Shaders.

	glShader* mDepthShader;//TODO: can use shader id.

	glShader* mOMDepthShader;

	glShader* mNormalShader;

	glShader* mPostRenderingShader;

	glShader* mBluringShader;

	glShader* mGBufferShader;
	glShader* mDeferredShader;

	MAT4 mLightSpaceMatrix;

	RenderData* mProcessMesh(jkMesh* mesh) override;

	InstanceRenderData* mProcessInstanceData(jkMesh* instanceMesh, std::vector<MAT4>& modelMatrices) override;

	CubeMapData* mProcessCubeMap(std::vector<unsigned char*>& faces, const ImageFormat& textureFormat) override;

	SurroundingRenderData* mProcessSurroundingData(std::vector<VEC3>& positions, Texture* tex) override;

	UINT mCreateTexture(Texture* pTexture);// Create texture  on GPU.

	void mRender(GLRenderData* pData);

	void mEnableLighting(GLRenderData* pData);

	void mRenderInstance(GLInstanceRenderData* instanceData);

	void mRenderSurrounding(GLSurroundingRenderData* surroundingData);

	void mGetDepthMap();

	void mGetDepthCubemap();

	void mCalcDepth(RenderData* pData, glShader* depthShader);

	void mCalcDepth(InstanceRenderData* pData, glShader* depthShader);

	void mRenderNormal(GLRenderData* pData);

	UINT mBlurRendering(UINT color_buffer);

	void mPostRendering(UINT color_buffer);// Render a squad, enable post-effects.

	void mRenderQuad();

	void mPrepareGBuffer();

	void mGetGBuffer(GLRenderData* pData);

	void mRenderWithGBuffer();

	void mCopyBufferData(UINT vbo_from, UINT vbo_target, UINT dataSize);

	void mUpdateViewPos();

	void mUpdateLightPos();

	void mUpdateViewMatrix();

	void mUpdateProjMatrix();

};
