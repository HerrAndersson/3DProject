#include "Application.h"
#include <iostream>
#include <DirectXCollision.h>

using namespace DirectX;
using namespace std;

Application::Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	Direct3D = new D3DClass(screenWidth, screenHeight, hwnd, false, 10000.0f, 0.1f);
	timer = new Timer();
	input = new InputHandler(hInstance, hwnd, screenWidth, screenHeight);
	position = new Position(XMFLOAT3(170.0f, 15.0f, 50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

	camera = new Camera();
	camera->SetPosition(position->GetPosition());
	camera->SetRotation(position->GetRotation());

	terrain = new Terrain(	Direct3D->GetDevice(), "assets/textures/terrain/heightmap02.bmp", 7.0f, 
							"assets/textures/terrain/blendmap.raw",
							"assets/textures/terrain/grass.raw",
							"assets/textures/terrain/stone.raw",
							"assets/textures/terrain/sand.raw");


	XMMATRIX tempWorldMatrix = XMMatrixIdentity();

	camel = new Object(Direct3D->GetDevice(), "assets/models/camel.obj", "assets/textures/camel.raw", tempWorldMatrix);
	wagon = new Object(Direct3D->GetDevice(), "assets/models/wagon.obj", "assets/textures/wagon.raw", tempWorldMatrix);
	particleEmitter = new ParticleEmitter(Direct3D->GetDevice(), "assets/textures/dollar.raw");

	spheres = new ObjectBase*[NUM_SPHERES];

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		XMFLOAT3 pos((float)(170 + (i * 20 + 5*i)), 20, 170);
		XMFLOAT3 scale((float)(8 + i), (float)(8 + i), (float)(8 + i));
		spheres[i] = new ObjectIntersection(Direct3D->GetDevice(), "assets/models/sphere3.obj", "assets/textures/missing.raw", pos, scale, XMMatrixIdentity());
	}

	sphere = new ObjectIntersection(Direct3D->GetDevice(), "assets/models/sphere3.obj", "assets/textures/missing.raw", XMFLOAT3(15, 5, 128), XMFLOAT3(10, 10, 10), tempWorldMatrix);

	modelQuadtree = new Quadtree(Direct3D->GetDevice(), "assets/models/tree.txt");


	// Initialize the light object.
	XMFLOAT4 ambient(0.05f, 0.05f, 0.05f, 1.0f);
	XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT3 direction(0.0f, -0.6f, 0.75f);
	//XMFLOAT3 direction(0.0f, 0.0f, 1.0f);

	light = new Light(ambient, diffuse, direction);
	orthoWindow = new OrthoWindow(Direct3D->GetDevice(), screenWidth, screenHeight);

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
	if (spheres)
	{
		for (int i = 0; i < NUM_SPHERES; i++)
		{
			delete spheres[i];
		}
		delete[] spheres;
	}
	if (sphere)
	{
		delete sphere;
	}
	if (modelQuadtree)
	{
		delete modelQuadtree;
		modelQuadtree = nullptr;
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

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		((ObjectIntersection*)spheres[i])->Update();
	}
	((ObjectIntersection*)sphere)->Update();

	//Check if the user pressed escape and wants to exit the application.
	if (input->Escape())
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

	keyDown = input->LMB();
	if (keyDown)
	{
		float closestDist = 9999999.0f;
		float distance = -1.0f;
		int closestIndex = -1;

		for (int i = 0; i < NUM_SPHERES; i++)
		{
			bool intersect = TestIntersections((ObjectIntersection*)spheres[i], distance);
			if (intersect)
			{
				if (distance < closestDist)
				{
					closestDist = distance;
					closestIndex = i;
				}
			}
		}

		if (closestIndex > -1)
		{
			XMFLOAT3 p = ((ObjectIntersection*)spheres[closestIndex])->GetPosition();
			((ObjectIntersection*)spheres[closestIndex])->SetPosition(XMFLOAT3(p.x, p.y + 10, p.z));
		}


		bool intersect = TestIntersections((ObjectIntersection*)sphere, distance);
		if (intersect)
		{
			cout << "INTERSECT" << endl;
			//system("cls");

			XMFLOAT3 p = ((ObjectIntersection*)sphere)->GetPosition();
			((ObjectIntersection*)sphere)->SetPosition(XMFLOAT3(p.x, p.y + 10, p.z));
			intersect = false;
		}
	}
}

Ray Application::GetRay()
{
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//Transform ray from view to world space
	XMMATRIX invView;
	XMVECTOR matInvDeter;

	XMMATRIX v;
	camera->GetViewMatrix(v);
	invView = XMMatrixInverse(&matInvDeter, v);

	XMVECTOR pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, invView);
	XMVECTOR pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, invView);

	XMFLOAT3 ro;
	XMFLOAT3 rd;
	XMStoreFloat3(&ro, pickRayInWorldSpacePos);
	XMStoreFloat3(&rd, pickRayInWorldSpaceDir);

	Ray r(ro, rd);

	return r;
}

bool Application::TestIntersections(ObjectIntersection* object, float& distance)
{
	bool intersect = false;

	Ray r = GetRay();
	RayVsSphere(r, *object->GetIntersectionSphere(), distance);

	if (distance > 0)
	{
		intersect = true;
	}

	return intersect;
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

	lightShader->SetBuffers(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix, Direct3D->GetDeferredSRV(0), Direct3D->GetDeferredSRV(1), light->GetDirection());
	lightShader->Draw(Direct3D->GetDeviceContext(), orthoWindow->GetIndexCount());

	Direct3D->TurnZBufferON();

	Direct3D->EndScene();

	return result;
}

void Application::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	Direct3D->ActivateDeferredShading();

	Direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);

	Direct3D->TurnOffCulling();

	//Render terrain
	terrainShader->UseShader(Direct3D->GetDeviceContext());
	terrainShader->SetBuffers(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, terrain->GetTextures(), camera->GetPosition());
	terrain->Render(Direct3D->GetDeviceContext());

	Direct3D->TurnOnCulling();

	//Render objects
	modelShader->UseShader(Direct3D->GetDeviceContext());
	XMMATRIX world = XMMatrixScaling(4, 4, 4)*XMMatrixTranslation(128, 1, 64);

	modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	camel->Render(Direct3D->GetDeviceContext());

	world = XMMatrixRotationRollPitchYaw(0, XMConvertToRadians(180), 0) * XMMatrixScaling(0.5, 0.5, 0.5) * XMMatrixTranslation(128, 1, 64);
	modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	wagon->Render(Direct3D->GetDeviceContext());

	modelQuadtree->Render(Direct3D->GetDeviceContext(), modelShader, viewMatrix, projectionMatrix);

	//XMFLOAT3 center = ((ObjectIntersection*)spheres)->GetIntersectionSphere()->center;
	//float radius = ((ObjectIntersection*)spheres)->GetIntersectionSphere()->radius;
	//world = XMMatrixScaling(radius, radius, radius)*XMMatrixTranslation(center.x, center.y, center.z);
	//modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	//((ObjectIntersection*)spheres)->RenderSphere(Direct3D->GetDeviceContext());

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		spheres[i]->GetWorldMatrix(world);
		modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
		spheres[i]->Render(Direct3D->GetDeviceContext());
	}

	sphere->GetWorldMatrix(world);
	modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	sphere->Render(Direct3D->GetDeviceContext());

	

	//Render particles
	particleShader->UseShader(Direct3D->GetDeviceContext());
	particleShader->SetMatrices(Direct3D->GetDeviceContext(), XMMatrixTranslation(128-30, 1, 64-30), viewMatrix, projectionMatrix, camera->GetPosition());
	particleEmitter->Render(Direct3D->GetDeviceContext());

	//Deactivate deferred shading. Activates forward shading again
	Direct3D->SetBackBufferRenderTarget();
	Direct3D->ResetViewport();
}

void Application::CreateShaders()
{
	modelShader = new ShaderDefault(Direct3D->GetDevice(), L"assets/shaders/ShaderUvVS.hlsl", L"assets/shaders/ShaderUvPS.hlsl");
	particleShader = new ShaderParticles(Direct3D->GetDevice(), L"assets/shaders/ShaderParticlesVS.hlsl", L"assets/shaders/ShaderParticlesPS.hlsl", L"assets/shaders/ShaderParticlesGS.hlsl");
	terrainShader = new ShaderTerrain(Direct3D->GetDevice(), L"assets/shaders/TerrainVS.hlsl", L"assets/shaders/TerrainPS.hlsl", L"assets/shaders/TerrainGS.hlsl");
	lightShader = new ShaderLight(Direct3D->GetDevice(), L"assets/shaders/LightVS.hlsl", L"assets/shaders/LightPS.hlsl");
}
