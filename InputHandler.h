#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <stdexcept>

using namespace DirectX;
using namespace std;

class InputHandler
{
private:

	IDirectInput8* directInput;
	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;

	unsigned char keyboardState[256];

	DIMOUSESTATE mouseState;

	int screenWidth;
	int screenHeight;

	XMFLOAT2 mousePos;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

public:

	InputHandler(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	~InputHandler();

	void Update();

	XMFLOAT2 GetMouseLocation();

	bool Escape();
	bool W();
	bool A();
	bool S();
	bool D();

	//Returns lX and lY from the mouse state
	XMFLOAT2 HandleMouse();

};

