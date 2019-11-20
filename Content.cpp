#include"Content.h"
#include"FrontendRendererWin32.h"
#include"BackendRendererWin32.h"
#include"MapManager.h"
#include<assert.h>

void jkContent::Init()
{
	// Start up renderers.
	switch (contentFrontendDevice)
	{
	case jkFrontendDevice::WIN_32:
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

	//jkMapManager::LoadMaps("");
	//SelectMap(jkMapManager::NextMap());

}

void jkContent::Display()
{
	//assert(currentMap);
	//frontendRenderer->RenderMap(currentMap);
}

void jkContent::StartUp()
{
	while (!ShouldFinish())
	{
		screen_dispatch();
		curTime = timeGetTime();
		float fps = 1 * 1000 / float(curTime - lastTime);
		//if (fps > max_fps) max_fps = fps;
		std::string fpsStr = std::to_string(fps);
		lastTime = curTime;
		::SetConsoleTitleA(fpsStr.c_str());
		//std::cout << fpsStr << '\r';
		renderer->Clear();

		// Input.
		for (auto it = content->input_name_map.begin(); it != content->input_name_map.end(); ++it)
		{
			auto key = (*it).first;
			auto key_id = BFContent::MapKey(key);
			if (key_id != -1 && BFContent::screen_keys[key_id] == 1)
			{
				//std::cout << key_id << std::endl;
				auto input = (*it).second;
				auto op = content->input_op_map.at(input);
				op();
			}
		}
		//cubeMesh->RotateWithY(1.f);
		//renderer->RenderMesh(cubeMesh);

		cuteMesh->RotateWithY(1.f);
		renderer->RenderMesh(cuteMesh);

		renderer->Display();

	}
}


