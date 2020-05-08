#include"Content.h"
#include"FreeCamera.h"

int main()
{
	jkContent& content = jkContent::GetInstance();
	content.SelectFrontendDevice();
	content.SelectBackendDevice();
	content.Init(800, 600);

	content.RegisterInput(jkInput::KEY_W, "forward");
	content.RegisterInput(jkInput::KEY_S, "backward");
	content.RegisterInput(jkInput::KEY_A, "left");
	content.RegisterInput(jkInput::KEY_D, "right");
	content.RegisterInput(jkInput::KEY_SPACE, "up");
	content.RegisterInput(jkInput::KEY_CTRL, "down");

	content.RegisterInput(jkInput::MOUSE_LEFT, "attack");
	content.RegisterInput(jkInput::MOUSE_RIGHT, "defend");
	content.RegisterInput(jkInput::MOUSE_WHEEL, "zoom");
	content.RegisterInput(jkInput::MOUSE_X, "turn");
	content.RegisterInput(jkInput::MOUSE_Y, "lookup");

	content.SetMouseSensi(0.4f);

	//content.

	auto pMap = new jkMap();

	//auto pEntity = new jkEntity({-5.f, 0.f, -5.f});
	//pEntity->MeshPath = "./Asset/model-triangulated.obj";
	//pMap->AddEntity(pEntity);

	auto pFreeCamera = new jkFreeCamera();
	pFreeCamera->GetCamera()->SetFarPlane(300.f);
	pMap->AddEntity(pFreeCamera);
	pMap->ControllCharacter(pFreeCamera);

	//content.SelectMapIndoor(pMap);
	content.SelectMapNew(pMap);

	content.StartUp();

	return 0;
}