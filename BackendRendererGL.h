#pragma once
#include "BackendRenderer.h"
#include "GLShader.h"
#include"glad/glad.h"
#include"Mesh.h"

struct GLRenderData : public RenderData
{
	GLRenderData() : RenderData(), VAO(0), VBO(0), EBO(0) {}
	~GLRenderData() 
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		// TODO : texture is not delete yet.
	}

	unsigned int VAO, VBO, EBO;
};

class jkBackendRendererGL : public jkBackendRenderer
{
public:

	jkBackendRendererGL(): m_pCurrentShader(nullptr){}

	void SetClearColor(COLOR3 clearColor = { 0.27f, 0.27f, 0.27f });

	void Clear() override;

	void StartUp() override;

	void LoadMesh(jkMesh* mesh) override;

	void StartRender() override;

	void DrawSkyBox(jkMesh* skyBoxMesh);

	UINT CreateTexture(Texture* pTexture);// Create texture  on GPU.


private:

	//void mPrepareRenderData(RenderData& data);

	void mRender(GLRenderData* pData);

	glShader* m_pCurrentShader;
};

