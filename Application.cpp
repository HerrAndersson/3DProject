#include "Application.h"
#include <iostream>

using namespace DirectX;
using namespace std;

Application::Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	Direct3D = new D3DClass(screenWidth, screenHeight, hwnd, false, 1000, 0.1f);
	timer = new Timer();
	input = new InputHandler(hInstance, hwnd, screenWidth, screenHeight);
	position = new Position(XMFLOAT3(125.0f, 15.0f, -15.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

	camera = new Camera();
	camera->SetPosition(position->GetPosition());
	camera->SetRotation(position->GetRotation());

	terrain = new Terrain(Direct3D->GetDevice(), "assets/textures/heightmap03.bmp", 8.5f);
	camel = new Object("assets/models/camel.obj", "assets/textures/camel.raw", Direct3D->GetDevice());
	particleEmitter = new ParticleEmitter(Direct3D->GetDevice(), "assets/textures/camel.raw");

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

	//MODELS
	if (camel)
	{
		delete camel;
		camel = nullptr;
	}
	if (particleEmitter)
	{
		delete particleEmitter;
		particleEmitter = nullptr;
	}

	//SHADERS
	if (terrainShader)
	{
		delete terrainShader;
		terrainShader = nullptr;
	}
	if (defaultShader)
	{
		delete defaultShader;
		defaultShader = nullptr;
	}
	if (particleShader)
	{
		delete particleShader;
		particleShader = nullptr;
	}
}

bool Application::Update()
{
	bool result = true;

	//Update the frameTime
	timer->Update();

	// Read the user input.
	input->Update();

	//Handle input from keyboard and mouse
	HandleMovement(timer->GetTime());

	//Update the camera. viewMatrix gets updated here
	camera->Update();

	// Render the graphics.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
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

	static_cast<ParticleEmitter*>(particleEmitter)->Update(Direct3D->GetDeviceContext(), frameTime);
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

	//Render terrain
	terrainShader->UseShader(Direct3D->GetDeviceContext());
	terrainShader->SetMatrices(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	terrain->Render(Direct3D->GetDeviceContext());


	//Render objects
	defaultShader->UseShader(Direct3D->GetDeviceContext());
	XMMATRIX world = XMMatrixScaling(4, 4, 4)*XMMatrixTranslation(128, 1, 64);
	//camel->GetWorldMatrix(world);
	defaultShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	camel->Render(Direct3D->GetDeviceContext());

	//Render particles
	particleShader->UseShader(Direct3D->GetDeviceContext());
	particleShader->SetMatrices(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	particleEmitter->Render(Direct3D->GetDeviceContext());

	Direct3D->EndScene();

	return result;
}

void Application::CreateShaders()
{
	terrainShader = new ShaderColor(Direct3D->GetDevice(), L"assets/shaders/vsTerrain.hlsl", L"assets/shaders/PixelShader.hlsl");
	defaultShader = new ShaderDefault(Direct3D->GetDevice(), L"assets/shaders/ShaderUvVS.hlsl", L"assets/shaders/ShaderUvPS.hlsl");
	particleShader = new ShaderParticles(Direct3D->GetDevice(), L"assets/shaders/ShaderParticlesVS.hlsl", L"assets/shaders/ShaderParticlesPS.hlsl", L"assets/shaders/ShaderParticlesGS.hlsl");
}
