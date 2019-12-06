#pragma once
#include "BackendRenderer.h"
#include "GLShader.h"
#include"glad/glad.h"
#include"Mesh.h"

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

class jkBackendRendererGL : public jkBackendRenderer
{
public:

	jkBackendRendererGL(){}

	void SetClearColor(COLOR3 clearColor = { 0.27f, 0.27f, 0.27f });

	void Clear() override;

	void StartUp() override;

	void LoadMesh(jkMesh* mesh) override;

	void StartRender() override;

	void DrawSkyBox(jkMesh* skyBoxMesh);

private:

	//void mPrepareRenderData(RenderData& data);

	UINT mCreateTexture(Texture* pTexture);// Create texture  on GPU.

	void mRender(GLRenderData* pData);

};

