#include"Content.h"
#include"FrontendRendererWin32.h"
#include"BackendRendererWin32.h"
#include"MapManager.h"
#include<assert.h>

void jkContent::StartUp()
{
	// Start up renderers.
	switch (contentFrontendDevice)
	{
	case jkFrontendDevice::WIN32:
		frontendRenderer = new jkFrontendRendererWin32();
		break;
	default:
		frontendRenderer = new jkFrontendRendererWin32();
		break;
	}

	switch (contentBackendDevice)
	{
	case jkBackendDevice::OPENGL:
		backendRenderer = new jkBackendRendererWin32();
		break;
	default:
		backendRenderer = new jkBackendRendererWin32();
		break;
	}

	jkMapManager::LoadMaps("");
	SelectMap(jkMapManager::NextMap());

}

void jkContent::Display()
{
	assert(currentMap);
	frontendRenderer->RenderMap(currentMap);
}
