#include"Content.h"
#include"Examples/FPSCharacterExample.h"

int main()
{
	jkContent& content = jkContent::GetInstance();
	content.SelectFrontendDevice();
	content.SelectBackendDevice();
	content.Init(800, 600);

	content.RegisterInput(jkSysInput::KEY_W, "forward");
	content.RegisterInput(jkSysInput::KEY_S, "backward");
	content.RegisterInput(jkSysInput::KEY_A, "left");
	content.RegisterInput(jkSysInput::KEY_D, "right");
	content.RegisterInput(jkSysInput::KEY_SPACE, "up");
	content.RegisterInput(jkSysInput::KEY_CTRL, "down");

	content.RegisterInput(jkSysInput::MOUSE_LEFT, "attack");
	content.RegisterInput(jkSysInput::MOUSE_RIGHT, "defend");
	//content.RegisterInput(jkSysInput::MOUSE_WHEEL, "zoom");
	content.RegisterInput(jkSysInput::MOUSE_X, "turn");
	content.RegisterInput(jkSysInput::MOUSE_Y, "lookup");

	content.SetMouseSensi(0.4f);

	//content.

	auto pMap = new jkMap();

	//auto pEntity = new jkEntity({-5.f, 0.f, -5.f});
	//pEntity->MeshPath = "./Asset/model-triangulated.obj";
	//pMap->AddEntity(pEntity);

	auto pFreeCamera = new jkExample::FPS::jkCharacterExample();
	pFreeCamera->GetCamera()->GetTransform().Translate({0, 1.f, 0}, 50.f)->RotatePitch(-45.f)->RotateYaw(45.f);
	pFreeCamera->GetCamera()->SetFarPlane(300.f);
	pMap->AddEntity(pFreeCamera);
	pMap->ControllCharacter(pFreeCamera);

	//content.SelectMapIndoor(pMap);
	content.SelectMapNew(pMap);

	content.StartUp();

	return 0;
}