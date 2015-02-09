#include "Application.h"
#include <iostream>

using namespace DirectX;
using namespace std;

Application::Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	Direct3D = nullptr;
	defaultShader = nullptr;
	camera = nullptr;
	terrain = nullptr;

	XMMATRIX baseViewMatrix;

	Direct3D = new D3DClass(screenWidth, screenHeight, hwnd, false, 1000, 0.1f);
	camera = new Camera();

	camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -0.5f));
	camera->Update();
	camera->GetViewMatrix(baseViewMatrix);
	camera->SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
	camera->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));

	terrain = new Terrain(Direct3D->GetDevice());

	CreateShaders();
}

Application::~Application()
{
	//OBJECTS
	if (Direct3D)
	{
		delete Direct3D;
		Direct3D = nullptr;
	}

	if (camera)
	{
		delete camera;
		camera = nullptr;
	}

	if (terrain)
	{
		delete terrain;
		terrain = nullptr;
	}

	//SHADERS
	if (defaultShader)
	{
		delete defaultShader;
		defaultShader = nullptr;
	}

	if (terrainShader)
	{
		delete terrainShader;
		terrainShader = nullptr;
	}
}

bool Application::Update()
{
	bool result = true;

	//// Read the user input.
	//result = Input->Update();
	//if (!result)
	//{
	//	return false;
	//}

	//// Check if the user pressed escape and wants to exit the application.
	//if (m_Input->IsEscapePressed() == true)
	//{
	//	return false;
	//}

	//// Update the system stats.
	//Timer->Frame();


	// Do the frame input processing.
	//result = HandleInput(Timer->GetTime());

	camera->Update();

	// Render the graphics.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}


bool Application::HandleInput(float frameTime)
{
	//get inputs from input class
	return true;
}


bool Application::RenderGraphics()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result = true;

	Direct3D->BeginScene(0.1f, 0.2f, 0.4f, 0.1f);

	camera->GetViewMatrix(viewMatrix);
	Direct3D->GetWorldMatrix(worldMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	terrainShader->UseShader(Direct3D->GetDeviceContext());
	terrainShader->SetMatrices(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);

	terrain->Render(Direct3D->GetDeviceContext());

	Direct3D->EndScene();

	return result;
}

void Application::CreateShaders()
{
	terrainShader = new ShaderColor(Direct3D->GetDevice(), L"assets/shaders/vsTerrain.hlsl", L"assets/shaders/PixelShader.hlsl");
}
