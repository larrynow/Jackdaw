#include"Content.h"
#include"FrontendRendererWin32.h"
#include"BackendRendererGL.h"
#include"WinInputManager.h"
#include"MapManager.h"
#include<assert.h>

void jkContent::Init(UINT width, UINT height)
{
	// Start up renderers.
	switch (mContentFrontendDevice)
	{
	case jkFrontendDevice::WIN_32:
		m_pFrontendRenderer = new jkFrontendRendererWin32();
		m_pInputManager = new jkWinInputManager();
		break;
	default:
		m_pFrontendRenderer = new jkFrontendRendererWin32();
		m_pInputManager = new jkWinInputManager();
		break;
	}

	switch (mContentBackendDevice)
	{
	case jkBackendDevice::OPENGL:
		m_pBackendRenderer = new jkBackendRendererGL();
		break;
	default:
		m_pBackendRenderer = new jkBackendRendererGL();
		break;
	}

	m_pFrontendRenderer->Init(width, height);

	// Choose a controll actor then bind input on it.

	// Note, the BindInput should be excuted in actor(or controller) class.
	//m_pInputManager->RegisterInput(BFInput::KEY_W, "forward");
	//m_pInputManager->BindInput("forward", [content]() {if (content->m_pControlledActor) content->m_pControlledActor->MoveForward(); });


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
		assert(m_pInputManager);
		m_pInputManager->Listen();
		
		/*::SetConsoleTitleA(fpsStr.c_str());
		renderer->Clear();*/

		// Input.
		for (auto it = m_pInputManager->input_name_map.begin(); it != m_pInputManager->input_name_map.end(); ++it)
		{
			auto input = (*it).first;
			auto input_id = m_pInputManager->GetInputId(input);
			if (input_id != -1 && *(m_pInputManager->KeyStatus+input_id) == 1)
			{
				auto input = (*it).second;
				auto op = m_pInputManager->input_op_map.at(input);
				op();
			}
		}

		m_pBackendRenderer->Display();

	}
}

bool jkContent::ShouldFinish()
{
	return jkInputManager::ExitStatus;
}


