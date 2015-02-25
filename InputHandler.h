#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <stdexcept>

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

	DirectX::XMFLOAT2 mousePos;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

public:

	InputHandler(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	~InputHandler();

	void Update();

	DirectX::XMFLOAT2 GetMouseLocation();

	bool Escape();
	bool W();
	bool A();
	bool S();
	bool D();

	//Returns lX and lY from the mouse state
	DirectX::XMFLOAT2 HandleMouse();

};

