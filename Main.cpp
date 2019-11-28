#include"Content.h"
#include"FreeCamera.h"

int main()
{
	jkContent content;
	content.SelectFrontendDevice();
	content.SelectBackendDevice();
	content.Init(800, 600);

	content.RegisterInput(jkInput::KEY_W, "forward");
	content.RegisterInput(jkInput::KEY_S, "backward");
	content.RegisterInput(jkInput::KEY_A, "left");
	content.RegisterInput(jkInput::KEY_D, "right");
	content.RegisterInput(jkInput::KEY_SPACE, "up");
	content.RegisterInput(jkInput::KEY_CTRL, "down");

	//content.

	auto pMap = new jkMap();

	auto pEntity = new jkEntity({0.f, 0.f, 50.f});
	pEntity->MeshPath = "./Asset/model-triangulated.obj";
	pMap->AddEntity(pEntity);

	auto pFreeCamera = new jkFreeCamera();
	pMap->AddEntity(pFreeCamera);
	pMap->ControllCharacter(pFreeCamera);

	content.SelectMap(pMap);
	
	content.StartUp();

	return 0;
}