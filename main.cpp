#include <windows.h>
#include "System.h"
#include <crtdbg.h>
#include "Console.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <stdio.h>

using namespace std;

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	RedirectIOToConsole();

	System* system;
	// Initialize and run the system object.
	try
	{
		system = new System();
		system->Run();
		// Shutdown and release the system object.
		delete system;
	}
	catch (exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_ICONERROR | MB_OK);
	}

	return 0;
}
