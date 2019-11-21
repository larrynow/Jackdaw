#pragma once
#include "BackendRenderer.h"
#include"glad/glad.h"

class jkBackendRendererGL : public jkBackendRenderer
{
public:

	void SetClearColor(COLOR3 clearColor = { 0.7f, 0.7f, 0.7f });

	void Clear() override;

	void StartUp() override;

	//void Display() override;
};

