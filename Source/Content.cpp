#include"Content.h"
#include"Jackdaw.h"
#include"FrontendRendererWin32.h"
#include"BackendRendererGL.h"
#include"WinInputManager.h"
#include"ClockWin32.h"
#include"MapManager.h"
#include"ResourceManager.h"
#include"Geometry.h"
#include"Terrain.h"
#include"Model.h"
#include<assert.h>

jkContent* jkSingleton<jkContent>::m_pInstance = nullptr;

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
		m_pTimer = jkClockWin32::GetInstancePtr();
		break;
	default:
		m_pFrontendRenderer = new jkFrontendRendererWin32();
		m_pInputManager = new jkWinInputManager();
		m_pTimer = jkClockWin32::GetInstancePtr();
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

	m_pTimer->Init();

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
	while (!mShouldFinish())
	{
		m_pTimer->Tick();
		PRINT(m_pTimer->GetFPS());
		/*::SetConsoleTitleA(fpsStr.c_str());
		renderer->Clear();*/
		m_pBackendRenderer->Clear();

		//////////////////////////////////
		// Input.
		for (auto it = m_pInputManager->input_name_map.begin(); it != m_pInputManager->input_name_map.end(); ++it)
		{
			auto input = (*it).first;
			auto input_id = m_pInputManager->MapKey(input);// Note only key.
			auto input_name = (*it).second;

			if (input_id != -1 && *(m_pInputManager->KeyStatus + input_id) == 1)
			{
				if (m_pControlledCharacter->input_op_map.count(input_name))
				{
					if (m_pControlledCharacter)// Check if character is still exist in content.
					{
						auto op = m_pControlledCharacter->input_op_map.at(input_name);
						op();
						//PRINT(input);
					}
				}
				
			}
		}

		////////////////////////////////
		// Fixed update.

		float real_time = m_pTimer->GetTime();
		//while (simulated_time < real_time)
		//{
		//	simulated_time += fixed_step;
		//	//UpdateFixed(fixed_step);
		//}

		////////////////////////////////
		// Rendering.

		// Update renderer matrices.
		mPrepareBackendRenderer();
		m_pBackendRenderer->StartRender();

		//m_pBackendRenderer->RenderGrass({2.0f, 0.0f, 0.0f});

		m_pFrontendRenderer->Display();

		assert(m_pInputManager);
		m_pInputManager->Listen();
	}
}

void jkContent::ChangeView()
{
	// When view changes, update backend view matrix.

}

bool jkContent::mShouldFinish()
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

	/*for (auto entity : map->mEntities)
	{
		jkResourceManager::ImportMeshFromOBJ(entity->MeshPath, entity->GetMesh());
	}*/

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
	jkResourceManager::LoadTexture("./Asset/awesomeface.bmp", texure, true);
	mesh->BindTexture(texure);
	m_pBackendRenderer->LoadMesh(mesh);

	// TODO : Warning! mesh should delete latter.

	///////////////////////////////////////////////
	// Load skybox.

	std::vector<unsigned char*> skyBoxFaces;
	ImageFormat skyboxTexFormat;
	jkResourceManager::ImportCubeMap(skyBoxFaces, skyboxTexFormat, "./Asset/skyBox/bluecloud", ".jpg");
	m_pBackendRenderer->SetUpSkybox(skyBoxFaces, skyboxTexFormat);
	// After setUp, skybox data is clear already.

	///////////////////////////////////////////////
	// Instance rocks.

	jkMesh* rockMesh = new jkMesh();
	jkResourceManager::ImportMeshFromOBJ("./Asset/rock.obj", rockMesh);

	Texture* rockTexture = new Texture();
	jkResourceManager::LoadTexture("./Asset/Rock-Texture-Surface.jpg", rockTexture);

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
		float scale = (rand() % 20) / 100.0f + 0.05f;
		MakeScaleMatrix(model, scale);

		// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
		float rotAngleRadian = GetRadian(float(rand() % 360));
		//model = model * Matrix_RotationXYZ(rotAngleRadian, rotAngleRadian, rotAngleRadian);
		model = Matrix_RotationXYZ(rotAngleRadian, rotAngleRadian, rotAngleRadian) * model;

		// 4. 添加到矩阵的数组中
		modelMatrices[i] = model.T();
	}
	m_pBackendRenderer->LoadInstanceData(rockMesh, modelMatrices);

	//////////////////////////////////////////////////////
	// Terrain.

	jkTerrain::Tile* terrain_tile = new jkTerrain::Tile();
	jkTerrain::CreateTerrain("./Asset/height_map.png", { 1024, 30, 1024 }, terrain_tile);

	// Load texture.
	auto soil_tex = jkResourceManager::ImportTexture("./Asset/soil.jpg");
	auto mud_tex = jkResourceManager::ImportTexture("./Asset/mud.jpg");
	auto grass_tex = jkResourceManager::ImportTexture("./Asset/grass.jpg");
	auto rock_tex = jkResourceManager::ImportTexture("./Asset/rock.jpg");
	auto sand_tex = jkResourceManager::ImportTexture("./Asset/sand.jpg");

	auto blend_tex = jkResourceManager::ImportTexture("./Asset/blend.jpg");

	//terrain_tile->pMesh->BindTexture(blend_tex);
	terrain_tile->pMesh->BindTexture(mud_tex);
	terrain_tile->pMesh->BindTexture(grass_tex);
	terrain_tile->pMesh->BindTexture(rock_tex);

	m_pBackendRenderer->LoadMesh(terrain_tile->pMesh);

	/////////////////////////////////////////////////
	// Dynamic grass.

	/*jkTerrain::
	m_pBackendRenderer->LoadSurroundingData(grassPositions);*/

	jkModel* model = new jkModel();
	jkResourceManager::LoadModel("./Asset/wolf/Wolf.fbx", model);

	auto wolf_body_tex = jkResourceManager::ImportTexture("./Asset/wolf/textures/Wolf_Body.jpg");
	auto wolf_body_flip_tex = jkResourceManager::ImportTexture("./Asset/wolf/textures/Wolf_Body.jpg",true);
	auto wolf_fur_tex = jkResourceManager::ImportTexture("./Asset/wolf/textures/Wolf_Fur.png");
	auto wolf_eye_1_tex = jkResourceManager::ImportTexture("./Asset/wolf/textures/Wolf_Eyes_1.jpg");
	auto wolf_eye_2_tex = jkResourceManager::ImportTexture("./Asset/wolf/textures/Wolf_Eyes_2.jpg");

	model->mMeshes[0]->BindTexture(wolf_body_tex);//belly fur.
	//m_pBackendRenderer->LoadMesh(model->mMeshes[0]);
	model->mMeshes[1]->BindTexture(wolf_body_tex);//paw.
	//m_pBackendRenderer->LoadMesh(model->mMeshes[1]);
	model->mMeshes[2]->BindTexture(wolf_eye_2_tex);//eye.
	//m_pBackendRenderer->LoadMesh(model->mMeshes[2]);
	model->mMeshes[3]->BindTexture(wolf_body_tex);//body.
	//m_pBackendRenderer->LoadMesh(model->mMeshes[3]);
	model->mMeshes[4]->BindTexture(wolf_body_tex);//head fur
	//m_pBackendRenderer->LoadMesh(model->mMeshes[4]);

	for (auto mesh : model->mMeshes)
	{
		mesh->RotateWithX(-90.f);
		m_pBackendRenderer->LoadMesh(mesh);
	}

	jkModel* nanosuit_model = new jkModel();
	jkResourceManager::LoadModel("./Asset/nanosuit/nanosuit.obj", nanosuit_model);

	auto arm_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/arm_dif.png");
	auto body_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/body_dif.png");
	auto glass_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/glass_dif.png");
	auto hand_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/hand_dif.png");
	auto helmet_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/helmet_dif.png");
	auto leg_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/leg_dif.png");

	nanosuit_model->mMeshes[0]->BindTexture(glass_tex);//glass
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[0]);
	nanosuit_model->mMeshes[1]->BindTexture(leg_tex);//leg
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[1]);
	nanosuit_model->mMeshes[2]->BindTexture(hand_tex);//hand
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[2]);
	nanosuit_model->mMeshes[3]->BindTexture(glass_tex);//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[3]);
	nanosuit_model->mMeshes[4]->BindTexture(arm_tex);//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[4]);
	nanosuit_model->mMeshes[5]->BindTexture(helmet_tex);//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[5]);
	nanosuit_model->mMeshes[6]->BindTexture(body_tex);//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[6]);

	for (auto mesh : nanosuit_model->mMeshes)
	{
		mesh->ScaleUpXYZ(0.1f);
		m_pBackendRenderer->LoadMesh(mesh);
	}

	//jkModel* amodel = new jkModel();
	//jkResourceManager::LoadModel("./Asset/wolf/Wolf_UDK_2.fbx", amodel);

}
