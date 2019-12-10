#include"Content.h"
#include"FrontendRendererWin32.h"
#include"BackendRendererGL.h"
#include"WinInputManager.h"
#include"MapManager.h"
#include"ResourceManager.h"
#include"Geometry.h"
#include<assert.h>

void jkContent::Init(UINT width, UINT height)
{
	mWidth = width;
	mHeight = height;

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

	m_pBackendRenderer->StartUp();
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

		/*::SetConsoleTitleA(fpsStr.c_str());
		renderer->Clear();*/
		m_pBackendRenderer->Clear();

		//////////////////////////////////
		// Input.
		for (auto it = m_pInputManager->input_name_map.begin(); it != m_pInputManager->input_name_map.end(); ++it)
		{
			auto input = (*it).first;
			auto input_id = m_pInputManager->MapKey(input);// Note only key.

			if (input_id != -1 && *(m_pInputManager->KeyStatus + input_id) == 1)
			{
				auto input = (*it).second;
				auto op = m_pControlledCharacter->input_op_map.at(input);
				if (m_pControlledCharacter)// Check if character is still exist in content.
				{
					op();
					//PRINT(input);
				}
			}
		}

		////////////////////////////////
		// Rendering.

		// Update renderer matrices.
		mPrepareBackendRenderer();
		m_pBackendRenderer->StartRender();

		m_pFrontendRenderer->Display();

		assert(m_pInputManager);
		m_pInputManager->Listen();
	}
}

void jkContent::ChangeView()
{
	// When view changes, update backend view matrix.

}

bool jkContent::ShouldFinish()
{
	return jkInputManager::ExitStatus;
}

void jkContent::mPrepareBackendRenderer()
{
	// Set View and projection matrices.
	m_pControlledCharacter->MakeViewMatrix(m_pBackendRenderer->GetViewMatrix());
	auto pCamera = m_pControlledCharacter->GetCamera();
	if(mContentBackendDevice == jkBackendDevice::OPENGL)
		MakePerspectiveMatrix_GL(m_pBackendRenderer->GetProjMatrix(), GetRadian(pCamera->GetFOV()/2), float(mWidth) / float(mHeight), pCamera->GetNearPlane(), pCamera->GetFarPlane());
	
}

void jkContent::SelectMap(jkMap* map)
{
	m_pCurrentMap = map; 
	m_pControlledCharacter = map->GetControlledCharacter();

	////////////////////////////////////////////////
	// Load Resources.

	for (auto entity : map->mEntities)
	{
		jkResourceManager::ImportMeshFromOBJ(entity->MeshPath, entity->GetMesh());
	}

	//////////////////////////////////////////////
	// Load mesh into renderer.

	//for (auto entity : m_pCurrentMap->mEntities)
	//{
	//	m_pBackendRenderer->LoadMesh(entity->GetMesh());
	//}

	// A test cube mesh.
	jkMesh* mesh = new jkMesh(VEC3(1.f, 1.0f, -1.f));
	jkGeometry::MakeCubeMesh(mesh, 1.f);
	Texture* texure = new Texture();
	jkResourceManager::ImportTexture("./Asset/awesomeface.bmp", texure);
	mesh->BindTexture(texure);
	m_pBackendRenderer->LoadMesh(mesh);

	// TODO : Warning! mesh should delete latter.

	///////////////////////////////////////////////
	// Load skybox.

	std::vector<unsigned char*> skyBoxFaces;
	ImageFormat skyboxTexFormat;
	jkResourceManager::ImportCubeMap(skyBoxFaces, skyboxTexFormat, "./Asset/skyBox/blue_nebular", ".jpg");
	m_pBackendRenderer->SetUpSkybox(skyBoxFaces, skyboxTexFormat);
	// After setUp, skybox data is clear already.

	///////////////////////////////////////////////
	// Instance rocks.

	jkMesh* rockMesh = new jkMesh();
	jkResourceManager::ImportMeshFromOBJ("./Asset/rock.obj", rockMesh);

	Texture* rockTexture = new Texture();
	jkResourceManager::ImportTexture("./Asset/Rock-Texture-Surface.jpg", rockTexture);

	rockMesh->BindTexture(rockTexture);

	unsigned int amount = 1000;
	std::vector<MAT4> modelMatrices;
	modelMatrices.resize(amount);
	float radius = 50.0;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		MAT4 model;
		// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		MakeTranslateMatrix(model, x, y, z);

		// 2. 缩放：在 0.05 和 0.25f 之间缩放
		float scale = (rand() % 20) / 100.0f + 0.05;
		MakeScaleMatrix(model, scale);

		// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
		float rotAngleRadian = GetRadian(rand() % 360);
		//model = model * Matrix_RotationXYZ(rotAngleRadian, rotAngleRadian, rotAngleRadian);
		model = Matrix_RotationXYZ(rotAngleRadian, rotAngleRadian, rotAngleRadian) * model;

		// 4. 添加到矩阵的数组中
		modelMatrices[i] = model.T();
	}
	m_pBackendRenderer->LoadInstanceData(rockMesh, modelMatrices);

}
