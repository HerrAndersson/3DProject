#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <windows.h>
#include "Application.h"

class System
{

private:

	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	Application* application;

	int screenWidth;
	int screenHeight;
	bool fullscreen;
	bool showCursor;

	bool Update();
	void InitializeWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

public:
	System(bool fullscreen = false, bool showCursor = true);
	~System();

	bool Initialize();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static System* applicationHandle = nullptr;

#endif
