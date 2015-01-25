#include <windows.h>
#include "System.h"
#include <crtdbg.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	bool result;

	// Initialize and run the system object.
	System* system = new System();

	if (!system)
		return 0;

	result = system->Initialize();

	if (result)
		system->Run();

	// Shutdown and release the system object.
	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}
