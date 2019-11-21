#include "BackendRendererGL.h"
#pragma comment (lib, "opengl32.lib")

void jkBackendRendererGL::SetClearColor(COLOR3 color)
{
	glClearColor(color.x, color.y, color.z, 1.0f);
}

void jkBackendRendererGL::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void jkBackendRendererGL::StartUp()
{
	
	
}
