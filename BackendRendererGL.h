#pragma once
#include "BackendRenderer.h"
#include "GLShader.h"
#include"glad/glad.h"
#include"Mesh.h"
#include"Geometry.h"

struct GLRenderData : public RenderData
{
	GLRenderData() : RenderData(), VAO(0), VBO(0), EBO(0), pShader(nullptr){}
	~GLRenderData() 
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		// TODO : texture is not delete yet.
	}

	UINT VAO, VBO, EBO;
	std::vector<UINT> TEXTURES;
	glShader* pShader;
};

struct GLInstanceRenderData : public GLRenderData, public InstanceRenderData
{
	UINT InstanceVBO;
	std::vector<MAT4> pModelMatrices;
};

struct GLCubeMapData : public CubeMapData, public GLRenderData{};

class jkBackendRendererGL : public jkBackendRenderer
{
public:

	jkBackendRendererGL(){}

	void SetClearColor(COLOR3 clearColor = { 0.27f, 0.27f, 0.27f });

	void Clear() override;

	void StartUp() override;

	void StartRender() override;

	void RenderSkybox() override;

private:

	UINT mMatrixUBO;

	RenderData* mProcessMesh(jkMesh* mesh) override;

	InstanceRenderData* mProcessInstanceData(jkMesh* instanceMesh, std::vector<MAT4>& modelMatrices) override;

	CubeMapData* mProcessCubeMap(std::vector<unsigned char*>& faces, const ImageFormat& textureFormat) override;

	UINT mCreateTexture(Texture* pTexture);// Create texture  on GPU.

	void mRender(GLRenderData* pData);

	void mRenderInstance(GLInstanceRenderData* instanceData);

	void mCopyBufferData(UINT vbo_from, UINT vbo_target, UINT dataSize);

	void mUpdateViewMatrix();

	void mUpdateProjMatrix();

};
