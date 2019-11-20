#include"Content.h"

int main()
{
	jkContent content;
	content.SelectFrontendDevice();
	content.SelectBackendDevice();
	content.Init(800, 600);

	content.StartUp();

	return 0;
}