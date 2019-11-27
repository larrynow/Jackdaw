#pragma once
#include "BackendRenderer.h"
#include"glad/glad.h"
#include"Mesh.h"

class jkBackendRendererGL : public jkBackendRenderer
{
public:

	void SetClearColor(COLOR3 clearColor = { 0.27f, 0.27f, 0.27f });

	void Clear() override;

	void StartUp() override;

	void Render(RenderData data) override;

private:

	void mPrepareRenderData(RenderData data);

	unsigned int VAO, VBO, EBO;

};

