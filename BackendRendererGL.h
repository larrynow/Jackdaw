#pragma once
#include "BackendRenderer.h"
#include "GLShader.h"
#include"glad/glad.h"
#include"Mesh.h"

class jkBackendRendererGL : public jkBackendRenderer
{
public:

	jkBackendRendererGL():VAO(0), VBO(0), EBO(0), m_pCurrentShader(nullptr){}

	void SetClearColor(COLOR3 clearColor = { 0.27f, 0.27f, 0.27f });

	void Clear() override;

	void StartUp() override;

	//void Render(RenderData& data) override;

	virtual void DrawMesh(jkMesh* mesh) override;

	void DrawSkyBox(jkMesh* skyBoxMesh);

	UINT CreateTexture(Texture* pTexture);// Create texture  on GPU.


private:

	//void mPrepareRenderData(RenderData& data);

	unsigned int VAO, VBO, EBO;

	glShader* m_pCurrentShader;
};

