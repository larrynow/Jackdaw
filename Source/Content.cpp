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
#include"Light.h"
#include"Terrain.h"
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

	m_pTerrainTileManager = new jkTerrain::jkTerrainManager();

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

		/////////////////////////////////
		// Terrain update.

		if (m_pTerrainTileManager->GetTile())
		{
			m_pTerrainTileManager->TileUpdate(m_pControlledCharacter->GetCamera()->GetPos(), MAT4());

			m_pBackendRenderer->ChangeDataIndices(m_pTerrainTileManager->GetMesh(),
				m_pTerrainTileManager->GetTerrainData());

			// Get instance matrices by Terrain update.
			m_pBackendRenderer->ChangeInstances(m_pCurrentMap->m_InstanceMeshes_temp[0],
				m_pTerrainTileManager->GetInstanceMatrices());

			m_pBackendRenderer->ChangeSurrounding(m_pTerrainTileManager->GetPositions());
		}

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
		MakePerspectiveMatrix_GL(m_pBackendRenderer->GetProjMatrix(), GetRadian(pCamera->GetFOV()/2), 
			float(mWidth) / float(mHeight), pCamera->GetNearPlane(), pCamera->GetFarPlane());
	// Set viewPos.
	m_pBackendRenderer->SetViewPos(m_pControlledCharacter->mPosition);
}

void jkContent::SelectMapWild(jkMap* map)
{
	m_pCurrentMap = map; 
	m_pControlledCharacter = map->GetControlledCharacter();

	jkResourceManager::ImportShader("simple", "./Shaders/shader.vs",
		"./Shaders/shader.fs");
	jkResourceManager::ImportShader("heightMapping", "./Shaders/heightMapping.vs",
		"./Shaders/heightMapping.fs");
	//auto shader = jkResourceManager::GetShader("simple");
	auto shader = jkResourceManager::GetShader("heightMapping");

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
	jkMesh* mesh = new jkMesh({ 5.f, 1.0f, 0.f });
	jkGeometry::MakeCubeMesh(mesh, 1.f);
	Texture* texure = new Texture();
	jkResourceManager::LoadTexture("./Asset/awesomeface.bmp", texure, true);
	mesh->BindTexture(texure);
	mesh->BindShader(shader);
	m_pBackendRenderer->LoadMesh(mesh);

	// A test cube mesh.
	mesh = new jkMesh(VEC3(0.f, -8.0f,0.f));
	jkGeometry::MakeCubeMesh(mesh, 10.f);
	texure = new Texture();
	jkResourceManager::LoadTexture("./Asset/awesomeface.bmp", texure, true);
	mesh->BindTexture(texure);
	mesh->BindShader(shader);
	m_pBackendRenderer->LoadMesh(mesh);

	// TODO : Warning! mesh should delete latter in resourceManager.

	///////////////////////////////////////////////
	// Load skybox.

	std::vector<unsigned char*> skyBoxFaces;
	ImageFormat skyboxTexFormat;
	jkResourceManager::ImportCubeMap(skyBoxFaces, skyboxTexFormat, "./Asset/skyBox/bluecloud", ".jpg");
	m_pBackendRenderer->SetUpSkybox(skyBoxFaces, skyboxTexFormat);
	// After setUp, skybox data is clear already.

	
	//////////////////////////////////////////////////////
	// Terrain.

	m_pTerrainTileManager->CreateTile();
	m_pTerrainTileManager->CreateTerrain("./Asset/height_map.png", { 1024, 30, 1024 });

	auto grd_tex = jkResourceManager::ImportTexture("./Asset/rock_mossy_albedo.png");
	auto grd_normal_tex = jkResourceManager::ImportTexture("./Asset/rock_mossy_normal.png", TextureType::Normal);
	auto grd_height_tex = jkResourceManager::ImportTexture("./Asset/rock_mossy_height.png", TextureType::Height);

	m_pTerrainTileManager->GetMesh()->BindMaterial(2);
	m_pTerrainTileManager->GetMesh()->BindTextures({ grd_tex, grd_normal_tex, grd_height_tex });
	m_pTerrainTileManager->GetMesh()->BindShader(shader);
	m_pTerrainTileManager->InitializeBlocks(16, 16);
	m_pBackendRenderer->LoadMesh(m_pTerrainTileManager->GetMesh());

	///////////////////////////////////////////////
	// Instance rocks.

	jkMesh* rockMesh = new jkMesh();
	jkResourceManager::ImportMeshFromOBJ("./Asset/rock.obj", rockMesh);
	Texture* rockTexture = jkResourceManager::ImportTexture(
		"./Asset/rock.jpg");

	rockMesh->BindTexture(rockTexture);

	map->m_InstanceMeshes_temp.push_back(rockMesh);

	unsigned int amount = 1000;
	std::vector<MAT4> modelMatrices(1, MAT4());
	//modelMatrices.resize(amount);
	//float radius = 50.0;
	//float offset = 2.5f;
	//for (unsigned int i = 0; i < amount; i++)
	//{
	//	MAT4 model;
	//	// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
	//	float angle = (float)i / (float)amount * 360.0f;
	//	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	//	float x = sin(angle) * radius + displacement;
	//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	//	float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
	//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
	//	float z = cos(angle) * radius + displacement;
	//	MakeTranslateMatrix(model, x, y, z);

	//	// 2. 缩放：在 0.05 和 0.25f 之间缩放
	//	float scale = (rand() % 20) / 100.0f + 0.05f;
	//	MakeScaleMatrix(model, scale);

	//	// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
	//	float rotAngleRadian = GetRadian(float(rand() % 360));
	//	//model = model * Matrix_RotationXYZ(rotAngleRadian, rotAngleRadian, rotAngleRadian);
	//	model = Matrix_RotationXYZ(rotAngleRadian, rotAngleRadian, rotAngleRadian) * model;

	//	// 4. 添加到矩阵的数组中
	//	modelMatrices[i] = model.T();
	//}
	m_pBackendRenderer->LoadInstanceData(rockMesh, modelMatrices);

	m_pTerrainTileManager->CreateInstances();

	auto branch_tex = jkResourceManager::ImportTexture("./Asset/branch.png");

	std::vector<VEC3> t_positions(1, VEC3(0.f));
	m_pBackendRenderer->LoadSurroundingData(t_positions, branch_tex);

	m_pTerrainTileManager->CreatePositions();
	
	//Tree.

	jkModel* tree_model = new jkModel({ 0.f, 0.f, 0.f });
	jkResourceManager::LoadModel("./Asset/trees9.3ds", tree_model);

	auto tree_tex = jkResourceManager::ImportTexture("./Asset/Mossy_Tr.jpg");
	//auto tree_tex = jkResourceManager::ImportTexture("./Asset/Walnut_L.jpg");
	/*for (auto mesh : tree_model->mMeshes)
	{
		mesh->BindShader(shader);
		m_pBackendRenderer->LoadMesh(mesh);
	}*/
	tree_model->mMeshes.at(0)->BindTexture(tree_tex);
	tree_model->mMeshes.at(0)->BindShader(shader);
	tree_model->mMeshes.at(0)->RotateWithX(-90.f);
	m_pBackendRenderer->LoadMesh(tree_model->mMeshes.at(0));

	jkModel* model = new jkModel({0.f, 0.f, 0.f});
	jkResourceManager::LoadModel("./Asset/wolf/Wolf.fbx", model);

	auto wolf_body_tex = jkResourceManager::ImportTexture("./Asset/wolf/textures/Wolf_Body.jpg");
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
	model->mMeshes[4] = nullptr;

	//m_pBackendRenderer->LoadMesh(model->mMeshes[4]);

	for (auto mesh : model->mMeshes)
	{
		if (!mesh)continue;
		mesh->RotateWithX(-90.f);
		mesh->BindShader(shader);
		//m_pBackendRenderer->LoadMesh(mesh);
	}

	jkModel* nanosuit_model = new jkModel();
	jkResourceManager::LoadModel("./Asset/nanosuit/nanosuit.obj", nanosuit_model);

	auto arm_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/arm_dif.png");
	auto body_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/body_dif.png");
	auto glass_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/glass_dif.png");
	auto hand_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/hand_dif.png");
	auto helmet_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/helmet_dif.png");
	auto leg_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/leg_dif.png");

	auto arm_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/arm_showroom_spec.png", TextureType::Specular);
	auto body_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/body_showroom_spec.png", TextureType::Specular);
	auto glass_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/glass_showroom_spec.png", TextureType::Specular);
	auto hand_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/hand_showroom_spec.png", TextureType::Specular);
	auto helmet_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/helmet_showroom_spec.png", TextureType::Specular);
	auto leg_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/leg_showroom_spec.png", TextureType::Specular);

	nanosuit_model->mMeshes[0]->BindMaterial({glass_tex, glass_spec_tex, 32});//glass
//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[0]);
	nanosuit_model->mMeshes[1]->BindMaterial({ leg_tex, leg_spec_tex, 32 });//leg
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[1]);
	nanosuit_model->mMeshes[2]->BindMaterial({ hand_tex, hand_spec_tex, 32 });//hand
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[2]);
	nanosuit_model->mMeshes[3]->BindMaterial({ glass_tex, glass_spec_tex, 32 });//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[3]);
	nanosuit_model->mMeshes[4]->BindMaterial({ arm_tex, arm_spec_tex, 32 });//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[4]);
	nanosuit_model->mMeshes[5]->BindMaterial({ helmet_tex, helmet_spec_tex, 32 });//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[5]);
	nanosuit_model->mMeshes[6]->BindMaterial({ body_tex, body_spec_tex, 32 });//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[6]);

	for (auto mesh : nanosuit_model->mMeshes)
	{
		mesh->ScaleUpXYZ(0.1f);
		mesh->BindShader(shader);
		//m_pBackendRenderer->LoadMesh(mesh);
	}

	jkMesh* sphereMesh = new jkMesh({ 20.f, 20.f, -20.f });
	sphereMesh->DisableLighting();
	sphereMesh->EnableBlooming();
	jkGeometry::MakeSphereMesh(sphereMesh, { 1.f, 1.f, 1.f });
	sphereMesh->BindTexture(grd_tex);
	sphereMesh->BindShader(shader);

	m_pBackendRenderer->LoadMesh(sphereMesh);

	m_pBackendRenderer->SetUpLight(new DirectionLight({-10.f, -10.f, 10.f}));
	//m_pBackendRenderer->SetUpLight(new PointLight({-1.f, 1.f, 2.f}));
	//m_pBackendRenderer->SetUpLight(new PointLight({-1.f, 3.f, 2.f}));
	//m_pBackendRenderer->SetUpLight(new SpotLight({0.f, 2.f, 2.f}));

}

void jkContent::SelectMapIndoor(jkMap* map)
{
	jkResourceManager::ImportShader("simple", "./Shaders/shader.vs",
		"./Shaders/shader.fs");
	jkResourceManager::ImportShader("heightMapping", "./Shaders/heightMapping.vs",
		"./Shaders/heightMapping.fs");
	//auto shader = jkResourceManager::GetShader("simple");
	auto shader = jkResourceManager::GetShader("heightMapping");

	m_pCurrentMap = map;
	m_pControlledCharacter = map->GetControlledCharacter();

	///////////////////////////////////////////////
	// Load skybox.

	std::vector<unsigned char*> skyBoxFaces;
	ImageFormat skyboxTexFormat;
	jkResourceManager::ImportCubeMap(skyBoxFaces, skyboxTexFormat, "./Asset/skyBox/bluecloud", ".jpg");
	m_pBackendRenderer->SetUpSkybox(skyBoxFaces, skyboxTexFormat);

	//Wall mesh.
	jkMesh* wall_mesh_front = new jkMesh({ 0.f, 40.f, -20.f });
	wall_mesh_front->RotateWithX(90.f)->RotateWithZ(90.f);
	jkGeometry::MakePlaneMesh(wall_mesh_front, 80.f, 80.f, 100, 100, 0.1f);
	jkMesh* wall_mesh_left = new jkMesh({ -40.f, 40.f, 0.f });
	wall_mesh_left->RotateWithX(90.f)->RotateWithY(90.f);
	jkGeometry::MakePlaneMesh(wall_mesh_left, 40.f, 80.f, 100, 100, 0.1f);
	jkMesh* wall_mesh_right = new jkMesh({ 40.f, 40.f, 0.f });
	wall_mesh_right->RotateWithX(90.f)->RotateWithY(-90.f);
	jkGeometry::MakePlaneMesh(wall_mesh_right, 40.f, 80.f, 100, 100, 0.1f);
	jkMesh* wall_mesh_back = new jkMesh({ 0.f, 40.f, 20.f });
	wall_mesh_back->RotateWithX(90.f)->RotateWithY(180.f);
	jkGeometry::MakePlaneMesh(wall_mesh_back, 80.f, 80.f, 100, 100, 0.1f);

	auto wall_tex = jkResourceManager::ImportTexture(
		"./Asset/brickwall.jpg");

	auto wall_normal_tex = jkResourceManager::ImportTexture(
		"./Asset/brickwall_normal.jpg", TextureType::Normal);

	auto wall_height_tex = jkResourceManager::ImportTexture(
		"./Asset/brickwall_height.png", TextureType::Height);

	std::vector<jkMesh*> wall_meshs = {
		wall_mesh_front, /*wall_mesh_back,*/ wall_mesh_left, wall_mesh_right };

	for (auto w_m : wall_meshs)
	{
		w_m->BindShader(shader);
		w_m->BindTexture(wall_tex);
		w_m->BindTexture(wall_normal_tex);
		//w_m->BindTexture(wall_height_tex);
		w_m->CalcTangentSpace();
		m_pBackendRenderer->LoadMesh(w_m);
	}

	//Ground mesh.
	jkMesh* ground_mesh = new jkMesh({ 0.f, 0.f, 0.f });
	jkGeometry::MakePlaneMesh(ground_mesh, 80.f, 80.f, 100, 100, 1.f);
	//auto grd_tex = jkResourceManager::ImportTexture("./Asset/cobblestone_albedo.png");
	auto grd_tex = jkResourceManager::ImportTexture("./Asset/concrete_albedo.png");
	ground_mesh->BindTexture(grd_tex);

	auto grd_normal_tex = jkResourceManager::ImportTexture("./Asset/concrete_normal.png", TextureType::Normal);
	ground_mesh->BindTexture(grd_normal_tex);

	auto grd_height_tex = jkResourceManager::ImportTexture("./Asset/concrete_height.png", TextureType::Height);
	ground_mesh->BindTexture(grd_height_tex);

	ground_mesh->BindShader(shader);
	ground_mesh->CalcTangentSpace();
	//ground_mesh->RotateWithX(90.f);
	m_pBackendRenderer->LoadMesh(ground_mesh);
	//m_pCurrentMap->AddMesh(ground_mesh);

	// A test cube mesh.
	auto cb_mesh = new jkMesh(VEC3(0.f, 0.5f, -17.f));
	//cb_mesh->RotateWithY(60.f);
	//cb_mesh->RotateWithZ(45.f);
	jkGeometry::MakeCubeMesh(cb_mesh, 1.f);
	auto cb_texure = new Texture();
	jkResourceManager::LoadTexture("./Asset/awesomeface.bmp", cb_texure, true);
	cb_mesh->BindTexture(cb_texure);
	cb_mesh->CalcTangentSpace();
	cb_mesh->BindShader(shader);
	//m_pCurrentMap->AddMesh(cb_mesh);
	m_pBackendRenderer->LoadMesh(cb_mesh);

	/*auto cb_mesh_2 = new jkMesh(*cb_mesh);
	cb_mesh_2->MoveTo({-2.f, 0.5f, 0.f});
	cb_mesh_2->CalcTangentSpace();
	m_pBackendRenderer->LoadMesh(cb_mesh_2);*/

	jkModel* nanosuit_model = new jkModel(VEC3(-3.f, 0.f, -18.f));
	jkResourceManager::LoadModel("./Asset/nanosuit/nanosuit.obj", nanosuit_model);

	auto arm_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/arm_dif.png");
	auto body_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/body_dif.png");
	auto glass_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/glass_dif.png");
	auto hand_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/hand_dif.png");
	auto helmet_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/helmet_dif.png");
	auto leg_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/leg_dif.png");

	auto arm_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/arm_showroom_spec.png", TextureType::Specular);
	auto body_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/body_showroom_spec.png", TextureType::Specular);
	auto hand_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/hand_showroom_spec.png", TextureType::Specular);
	auto helmet_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/helmet_showroom_spec.png", TextureType::Specular);
	auto leg_spec_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/leg_showroom_spec.png", TextureType::Specular);

	auto arm_n_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/arm_showroom_ddn.png", TextureType::Normal);
	auto body_n_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/body_showroom_ddn.png", TextureType::Normal);
	auto glass_n_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/glass_ddn.png", TextureType::Normal);
	auto hand_n_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/hand_showroom_ddn.png", TextureType::Normal);
	auto helmet_n_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/helmet_showroom_ddn.png", TextureType::Normal);
	auto leg_n_tex = jkResourceManager::ImportTexture("./Asset/nanosuit/leg_showroom_ddn.png", TextureType::Normal);

	nanosuit_model->mMeshes[0]->BindMaterial({ glass_tex, glass_tex, 32 });//glass
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[0]);
	nanosuit_model->mMeshes[1]->BindMaterial({ leg_tex, leg_spec_tex, 32 });//leg
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[1]);
	nanosuit_model->mMeshes[2]->BindMaterial({ hand_tex, hand_spec_tex, 32 });//hand
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[2]);
	nanosuit_model->mMeshes[3]->BindMaterial({ glass_tex, glass_tex, 32 });//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[3]);
	nanosuit_model->mMeshes[4]->BindMaterial({ arm_tex, arm_spec_tex, 32 });//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[4]);
	nanosuit_model->mMeshes[5]->BindMaterial({ helmet_tex, helmet_spec_tex, 32 });//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[5]);
	nanosuit_model->mMeshes[6]->BindMaterial({ body_tex, body_spec_tex, 32 });//belly fur.
	//m_pBackendRenderer->LoadMesh(nanosuit_model->mMeshes[6]);

	nanosuit_model->mMeshes[0]->BindTexture(glass_n_tex);//glass
	nanosuit_model->mMeshes[1]->BindTexture(leg_n_tex);//glass
	nanosuit_model->mMeshes[2]->BindTexture(hand_n_tex);//glass
	nanosuit_model->mMeshes[3]->BindTexture(glass_n_tex);//glass
	nanosuit_model->mMeshes[4]->BindTexture(arm_n_tex);//glass
	nanosuit_model->mMeshes[5]->BindTexture(helmet_n_tex);//glass
	nanosuit_model->mMeshes[6]->BindTexture(body_n_tex);//glass

	for (size_t i = 0; i < nanosuit_model->mMeshes.size(); i++)
	{
		auto mesh = nanosuit_model->mMeshes[i];
		mesh->BindShader(shader);
		mesh->ScaleUpXYZ(0.1f);
		mesh->RotateWithY(45.f);
		mesh->CalcTangentSpace(true);
		
		m_pBackendRenderer->LoadMesh(mesh);
		map->AddMesh(mesh);

	}
	jkMesh* sphereMesh = new jkMesh({-2.f, 2.f, -18.f});
	sphereMesh->DisableLighting();
	sphereMesh->EnableBlooming();
	jkGeometry::MakeSphereMesh(sphereMesh, {10.f, 10.f, 10.f});
	sphereMesh->ScaleUpXYZ(0.2);
	sphereMesh->BindTexture(grd_tex);
	sphereMesh->BindShader(shader);

	m_pBackendRenderer->LoadMesh(sphereMesh);

	//m_pBackendRenderer->SetUpLight(new DirectionLight({ -20.f, -20.f, 0.f }));

	m_pBackendRenderer->SetUpLight(new PointLight(VEC3(0.15f), VEC3(1.8f), VEC3(5.0f), VEC3( -2.f, 2.f, -18.f ), 1.f, 0.35f, 0.44f));

	//m_pBackendRenderer->SetUpLight(new PointLight({ 0.f, 4.f, -8.f }));

}