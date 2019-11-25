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

	m_pFrontendRenderer->Init(width, height, mContentBackendDevice);

	m_pBackendRenderer->SetClearColor();

	//jkMapManager::LoadMaps("");
	//m_pCurrentMap = jkMapManager::NextMap();

	//m_pControlledCharacter = m_pCurrentMap->GetControlledCharacter();

	////TODO : register input from a inpout table.
	//m_pInputManager->RegisterInput(jkInput::KEY_W, "forward");


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

		m_pBackendRenderer->Clear();

		// Input.
		for (auto it = m_pInputManager->input_name_map.begin(); it != m_pInputManager->input_name_map.end(); ++it)
		{
			auto input = (*it).first;
			auto input_id = m_pInputManager->MapKey(input);// Note only key.

			if (input_id != -1 && *(m_pInputManager->KeyStatus+input_id) == 1)
			{
				auto input = (*it).second;
				auto op = m_pControlledCharacter->input_op_map.at(input);
				if (m_pControlledCharacter)// Check if character is still exist in content.
				{
					op(); PRINT(input);
				}
			}
		}

		m_pFrontendRenderer->Display();

	}
}

bool jkContent::ShouldFinish()
{
	return jkInputManager::ExitStatus;
}


