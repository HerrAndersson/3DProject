#pragma once

#include <windows.h>
#include <stdexcept>
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

	int realScreenWidth;
	int realScreenHeight;

	bool Update();
	void InitializeWindows();
	void ShutdownWindows();

public:
	System(bool fullscreen = false, bool showCursor = true, int screenWidth = 1000, int screenHeight = 800);
	~System();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static System* applicationHandle = nullptr;
