#include "Application.h"
#include <iostream>

using namespace DirectX;
using namespace std;

Application::Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	Direct3D = new D3DClass(screenWidth, screenHeight, hwnd, false, 1000, 0.1f);
	timer = new Timer();
	input = new InputHandler(hInstance, hwnd, screenWidth, screenHeight);
	position = new Position(XMFLOAT3(170.0f, 15.0f, 50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

	camera = new Camera();
	camera->SetPosition(position->GetPosition());
	camera->SetRotation(position->GetRotation());
	camera->CreateBaseViewMatrix();

	terrain = new Terrain(	Direct3D->GetDevice(), "assets/textures/terrain/heightmap02.bmp", 7.0f, 
							"assets/textures/terrain/blendmap.raw",
							"assets/textures/terrain/grass.raw",
							"assets/textures/terrain/stone.raw",
							"assets/textures/terrain/sand.raw");
	camel = new Object("assets/models/camel.obj", "assets/textures/camel.raw", Direct3D->GetDevice());
	wagon = new Object("assets/models/wagon.obj", "assets/textures/wagon.raw", Direct3D->GetDevice());
	particleEmitter = new ParticleEmitter(Direct3D->GetDevice(), "assets/textures/dollar.raw");

	// Initialize the light object.
	XMFLOAT4 ambient(0.05f, 0.05f, 0.05f, 1.0f);
	XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT3 direction(0.0f, -0.6f, 0.75f);
	//XMFLOAT3 direction(0.0f, 0.0f, 1.0f);

	light = new Light(ambient, diffuse, direction);
	orthoWindow = new OrthoWindow(Direct3D->GetDevice(), screenWidth, screenHeight);

	CreateShaders(screenHeight, screenWidth);
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
	if (timer)
	{
		delete timer;
		timer = nullptr;
	}
	if (input)
	{
		delete input;
		input = nullptr;
	}
	if (input) 
	{ 
		delete input; 
		input = nullptr; 
	}
	if (position)
	{
		delete position;
		position = nullptr;
	}
	if (light)
	{
		delete light;
		light = nullptr;
	}
	if (orthoWindow)
	{
		delete orthoWindow;
		orthoWindow = nullptr;
	}

	//MODELS
	if (camel)
	{
		delete camel;
		camel = nullptr;
	}
	if (wagon)
	{
		delete wagon;
		wagon = nullptr;
	}
	if (particleEmitter)
	{
		delete particleEmitter;
		particleEmitter = nullptr;
	}

	//SHADERS
	if (modelShader)
	{
		delete modelShader;
		modelShader = nullptr;
	}
	if (particleShader)
	{
		delete particleShader;
		particleShader = nullptr;
	}
	if (deferredShader)
	{
		delete deferredShader;
		deferredShader = nullptr;
	}
	if (lightShader)
	{
		delete lightShader;
		lightShader = nullptr;
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

	timer->Update();
	float frameTime = timer->GetTime();

	input->Update();

	HandleMovement(frameTime);

	//Update the camera. viewMatrix gets updated here.
	camera->Update();

	particleEmitter->Update(Direct3D->GetDeviceContext(), frameTime);

	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	//Check if the user pressed escape and wants to exit the application.
	if (input->Escape() == true)
	{
		result = false;
	}
	return result;
}


void Application::HandleMovement(float frameTime)
{
	bool keyDown;
	
	position->SetFrameTime(frameTime);

	//Handle the input.
	keyDown = input->W();
	position->MoveForward(keyDown);

	keyDown = input->A();
	position->MoveLeft(keyDown);

	keyDown = input->S();
	position->MoveBackward(keyDown);

	keyDown = input->D();
	position->MoveRight(keyDown);

	position->LookAround(input->HandleMouse());

	//Get the view point position/rotation.
	XMFLOAT3 pos = position->GetPosition();
	XMFLOAT3 rot = position->GetRotation();

	//Locking the Y-position to the ground
	pos.y = terrain->GetY(pos.x, pos.z) + HEIGHT_FROM_GROUND;

	// Set the position of the camera.
	camera->SetPosition(pos);
	camera->SetRotation(rot);
}


bool Application::RenderGraphics()
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result = true;

	RenderToTexture();

	Direct3D->BeginScene(0.2f, 0.4f, 1.0f, 1.0f);

	camera->GetViewMatrix(viewMatrix);
	Direct3D->GetWorldMatrix(worldMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	Direct3D->TurnZBufferOFF();

	orthoWindow->Render(Direct3D->GetDeviceContext());

	lightShader->SetBuffers(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix, deferredShader->GetShaderResourceView(0), deferredShader->GetShaderResourceView(1), light->GetDirection());
	lightShader->Draw(Direct3D->GetDeviceContext(), orthoWindow->GetIndexCount());

	Direct3D->TurnZBufferON();

	Direct3D->EndScene();

	return result;
}

void Application::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	deferredShader->SetRenderTargets(Direct3D->GetDeviceContext());
	deferredShader->ClearRenderTargets(Direct3D->GetDeviceContext(), 0.2f, 0.4f, 1.0f, 1.0f);

	Direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);

	//Render terrain
	terrainShader->UseShader(Direct3D->GetDeviceContext());
	terrainShader->SetBuffers(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, terrain->GetTextures());
	terrain->Render(Direct3D->GetDeviceContext());

	//Render objects
	modelShader->UseShader(Direct3D->GetDeviceContext());
	XMMATRIX world = XMMatrixScaling(4, 4, 4)*XMMatrixTranslation(128, 1, 64);

	modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	camel->Render(Direct3D->GetDeviceContext());

	world = XMMatrixRotationRollPitchYaw(0, XMConvertToRadians(180), 0) * XMMatrixScaling(0.5, 0.5, 0.5) * XMMatrixTranslation(128, 1, 64);
	modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	wagon->Render(Direct3D->GetDeviceContext());

	//Render particles
	particleShader->UseShader(Direct3D->GetDeviceContext());
	particleShader->SetMatrices(Direct3D->GetDeviceContext(), XMMatrixTranslation(128-30, 1, 64-30), viewMatrix, projectionMatrix, camera->GetPosition());
	particleEmitter->Render(Direct3D->GetDeviceContext());

	Direct3D->SetBackBufferRenderTarget();
	Direct3D->ResetViewport();
}

void Application::CreateShaders(int screenHeight, int screenWidth)
{
	modelShader = new ShaderDefault(Direct3D->GetDevice(), L"assets/shaders/ShaderUvVS.hlsl", L"assets/shaders/ShaderUvPS.hlsl");
	particleShader = new ShaderParticles(Direct3D->GetDevice(), L"assets/shaders/ShaderParticlesVS.hlsl", L"assets/shaders/ShaderParticlesPS.hlsl", L"assets/shaders/ShaderParticlesGS.hlsl");
	terrainShader = new ShaderTerrain(Direct3D->GetDevice(), L"assets/shaders/TerrainVS.hlsl", L"assets/shaders/TerrainPS.hlsl");
	deferredShader = new ShaderDeferred(Direct3D->GetDevice(), L"assets/shaders/DeferredVS.hlsl", L"assets/shaders/DeferredPS.hlsl", screenWidth, screenHeight);
	lightShader = new ShaderLight(Direct3D->GetDevice(), L"assets/shaders/LightVS.hlsl", L"assets/shaders/LightPS.hlsl");
}
