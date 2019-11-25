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

	auto pMap = new jkMap();
	auto pFreeCamera = new jkFreeCamera();
	pMap->AddEntity(pFreeCamera);
	pMap->ControllCharacter(pFreeCamera);

	content.SelectMap(pMap);

	content.StartUp();

	return 0;
}