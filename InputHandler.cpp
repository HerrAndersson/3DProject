#include "InputHandler.h"
#include <iostream>

using namespace std;
using namespace DirectX;

InputHandler::InputHandler(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	mousePos = XMFLOAT2(0.0f, 0.0f);

	//Initialize the main direct input interface
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(result))
	{
		throw runtime_error("Could not initialize main direct input interface");
	}

	//Initialize the interface for the keyboard
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(result))
	{
		throw runtime_error("Could not initialize the keyboard");
	}
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	result = keyboard->Acquire();
	if (FAILED(result))
	{
		throw runtime_error("Could not aquire the keyboard");
	}

	// Initialize the direct input interface for the mouse.
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(result))
	{
		throw runtime_error("Could not initialize the mouse");
	}
	mouse->SetDataFormat(&c_dfDIMouse);
	mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	result = mouse->Acquire();
	if (FAILED(result))
	{
		throw runtime_error("Could not aquire the mouse");
	}
}

InputHandler::~InputHandler()
{
	if (directInput)
	{
		directInput->Release();
		directInput = nullptr;
	}

	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = nullptr;
	}

	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = nullptr;
	}

}

void InputHandler::Update()
{
	bool result = false;

	result = ReadKeyboard();
	if (!result)
		return;

	result = ReadMouse();
	if (!result)
		return;

	ProcessInput();
}

bool InputHandler::ReadKeyboard()
{
	HRESULT result;
	bool r = false;

	//Read the keyboard device
	result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	//If the keyboard was lost or not acquired, try to get control back
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			keyboard->Acquire();
		}
	}
	else
	{
		r = true;
	}

	return r;
}
bool InputHandler::ReadMouse()
{
	HRESULT result;
	bool r = false;

	//Read the mouse device
	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);

	//If the mouse was lost or not acquired, try to get control back
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mouse->Acquire();
		}
	}
	else
	{
		r = true;
	}

	return r;
}
void InputHandler::ProcessInput()
{
	// Update the location of the mouse cursor
	mousePos.x += mouseState.lX;
	mousePos.y += mouseState.lY;

	//Ensure the mousePos is limited to the bounds of the window
	if (mousePos.x < 0)  { mousePos.x = 0; }
	if (mousePos.y < 0)  { mousePos.y = 0; }
	if (mousePos.x  > screenWidth)  { mousePos.x = (float)screenWidth; }
	if (mousePos.y  > screenHeight) { mousePos.y = (float)screenHeight; }
}

XMFLOAT2 InputHandler::GetMouseLocation()
{
	return mousePos;
}

bool InputHandler::Escape()
{
	bool pressed = false;
	if (keyboardState[DIK_ESCAPE] & 0x80)
	{
		pressed = true;
	}

	return pressed;
}
bool InputHandler::W()
{
	bool pressed = false;
	if (keyboardState[DIK_W] & 0x80)
	{
		pressed = true;
	}

	return pressed;
}
bool InputHandler::A()
{
	bool pressed = false;
	if (keyboardState[DIK_A] & 0x80)
	{
		pressed = true;
	}

	return pressed;
}
bool InputHandler::S()
{
	bool pressed = false;
	if (keyboardState[DIK_S] & 0x80)
	{
		pressed = true;
	}

	return pressed;
}
bool InputHandler::D()
{
	bool pressed = false;
	if (keyboardState[DIK_D] & 0x80)
	{
		pressed = true;
	}

	return pressed;
}

XMFLOAT2 InputHandler::HandleMouse()
{
	DIMOUSESTATE mouseCurrState;
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	//If these values are changed it means that actual mouse movement happened.
	XMFLOAT2 lxly(1000000.0f, 1000000.0f);

	//Check if the state of the mouse has changed since last time.
	if ((mouseCurrState.lX != mouseState.lX) || (mouseCurrState.lY != mouseState.lY))
	{
		lxly.x = (float)mouseState.lX;
		lxly.y = (float)mouseState.lY;
	}
	return lxly;
}

bool InputHandler::LMB()
{
	bool down = false;

	// Check if the left mouse button is currently pressed.
	if (mouseState.rgbButtons[0] & 0x80)
	{
		down = true;
	}

	return down;
}
