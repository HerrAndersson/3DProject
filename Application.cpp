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

	camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -1.0f));
	camera->Update();
	camera->GetViewMatrix(baseViewMatrix);
	camera->SetPosition(XMFLOAT3(50.0f, 20.0f, -7.0f));
	camera->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));

	terrain = new Terrain(Direct3D->GetDevice());

	CreateShaders(hwnd);
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

	camera->Update();
	camera->GetViewMatrix(viewMatrix);
	Direct3D->GetWorldMatrix(worldMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	terrainShader->UseShader(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	terrain->Render(Direct3D->GetDeviceContext());

	cout << camera->GetPosition() << endl;

	Direct3D->EndScene();

	return result;
}

bool Application::CreateShaders(HWND hwnd)
{
	bool result = true;

	D3D11_INPUT_ELEMENT_DESC defaultInputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Create Default shader object.
	defaultShader = new ShaderBase(Direct3D->GetDevice(), hwnd, defaultInputDesc, ARRAYSIZE(defaultInputDesc), L"assets/shaders/VertexShader.hlsl", L"assets/shaders/PixelShader.hlsl");
	if (!defaultShader)
	{
		throw runtime_error("Could not initialize Default shader.");
	}

	D3D11_INPUT_ELEMENT_DESC colorInputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	terrainShader = new ShaderColor(Direct3D->GetDevice(), hwnd, colorInputDesc, ARRAYSIZE(colorInputDesc), L"assets/shaders/vsTerrain.hlsl", L"assets/shaders/PixelShader.hlsl");
	if (!terrainShader)
	{
		throw runtime_error("Could not initialize Terrainshader.");
	}

	return result;
}
