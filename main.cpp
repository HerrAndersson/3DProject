#include <windows.h>
#include "System.h"
#include <crtdbg.h>
#include "Console.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vld.h> 

using namespace std;

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	srand((unsigned)time(NULL));

	RedirectIOToConsole();

	System* system;

	try
	{
		system = new System(false, false, 1440, 900);
		system->Run();
		delete system;
	}
	catch (exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_ICONERROR | MB_OK);
	}

	return 0;
}
