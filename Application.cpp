#include "Application.h"
#include <iostream>
#include <DirectXCollision.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

Application::Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	float screenDepth = 1000.0f;
	float screenNear = 0.1f;

	Direct3D = new D3DClass(screenWidth, screenHeight, hwnd, false, screenDepth, screenNear);
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


	particleEmitter = new ParticleEmitter(Direct3D->GetDevice(), "assets/textures/dollar.raw");

	spheres = new ObjectBase*[NUM_SPHERES];

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		XMFLOAT3 pos((float)(170 + (i * 20 + 5 * i)), 20, 170);
		XMFLOAT3 scale((float)(4 + i*i), (float)(4 + i*i), (float)(4 + i*i));
		spheres[i] = new ObjectIntersection(Direct3D->GetDevice(), "assets/models/sphere3.obj", pos, scale, XMMatrixIdentity());
	}

	XMMATRIX tempWorldMatrix = XMMatrixIdentity();
	sphere = new ObjectIntersection(Direct3D->GetDevice(), "assets/models/sphere3.obj", XMFLOAT3(15, 5, 128), XMFLOAT3(5, 5, 5), tempWorldMatrix);

	modelQuadtree = new Quadtree(Direct3D->GetDevice(), "assets/map/tree.txt");


	// Initialize the light object.
	XMFLOAT4 ambient(0.05f, 0.05f, 0.05f, 1.0f);
	XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT3 direction(0.0f, -0.6f, 0.75f);
	//XMFLOAT3 direction(0.0f, 0.0f, 1.0f);

	orthoWindow = new OrthoWindow(Direct3D->GetDevice(), screenWidth, screenHeight);

	XMFLOAT3 position(256.0f, 256.0f, -256.0f);
	shadowLight = new ShadowLight(ambient, diffuse, position, XMFLOAT3(256.0f, 0.0f, 256.0f));
	shadowLight->CreateProjectionMatrix(screenDepth, screenNear);
	shadowLight->CreateViewMatrix();

	light = new Light(ambient, diffuse, shadowLight->GetDirection());
	XMFLOAT3 dir = shadowLight->GetDirection();
	light->SetDirection(dir.x, dir.y, dir.z);

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
	if (shadowLight)
	{
		delete shadowLight;
		shadowLight = nullptr;
	}

	//MODELS
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
	if (shadowMapShader)
	{
		delete shadowMapShader;
		shadowMapShader = nullptr;
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

	//Uncomment if the shadow light is moving
	//XMFLOAT3 dir = shadowLight->GetDirection();
	//light->SetDirection(dir.x, dir.y, dir.z);

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
		bool intersect = false;

		for (int i = 0; i < NUM_SPHERES; i++)
		{
			intersect = TestIntersections((ObjectIntersection*)spheres[i], distance);
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

		distance = -1;
		intersect = TestIntersections((ObjectIntersection*)sphere, distance);
		if (intersect)
		{
			cout << "INTERSECT" << endl;

			XMFLOAT3 p = camera->GetPosition();
			((ObjectIntersection*)sphere)->SetPosition(XMFLOAT3(p.x, p.y, p.z));
		}
	}

	//Uncomment if the scene is rendered from another view, ex. from the light. 
	//XMFLOAT3 p = camera->GetPosition();
	//((ObjectIntersection*)sphere)->SetPosition(XMFLOAT3(p.x, p.y, p.z));
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

	//XMVECTOR origin = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//XMVECTOR dir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//XMMATRIX viewMx;
	//camera->GetViewMatrix(viewMx);

	//XMMATRIX modelMx;
	//object->GetWorldMatrix(modelMx);

	//XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(viewMx), viewMx);
	//XMMATRIX invModel = XMMatrixInverse(&XMMatrixDeterminant(modelMx), modelMx);
	//XMMATRIX toLocal = invView * invModel;

	//origin = XMVector3TransformCoord(origin, toLocal);
	//dir = XMVector3TransformNormal(dir, toLocal);
	//dir = XMVector3Normalize(dir);


	//XMMATRIX w, v, p;
	//Direct3D->GetProjectionMatrix(p);
	//camera->GetViewMatrix(v);
	//object->GetWorldMatrix(w);

	//XMVECTOR o = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//XMVECTOR d = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	//o = XMVector3Unproject(o, 0, 0, screenWidth, screenHeight, 0, 1, p, v, w);
	//d = XMVector3Unproject(d, 0, 0, screenWidth, screenHeight, 0, 1, p, v, w);

	//XMFLOAT3 ro;
	//XMFLOAT3 rd;
	//XMStoreFloat3(&ro, o);
	//XMStoreFloat3(&rd, d);

	//DirectX::BoundingSphere b;
	//b.Center = object->GetIntersectionSphere()->center;
	//b.Radius = object->GetIntersectionSphere()->radius;

	//intersect = b.Intersects(o, d, distance);
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
	XMMATRIX w, v, p, lightVP;
	shadowLight->GetViewMatrix(v);
	shadowLight->GetProjectionMatrix(p);
	XMFLOAT3 po = shadowLight->GetPosition();
	w = XMMatrixTranslation(po.x, po.y, po.z);

	lightVP = v * p;
	//lightVP = w * v * p;
	//lightVP = XMMatrixInverse(&XMMatrixDeterminant(lightVP), lightVP);

	lightShader->SetBuffers(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix,
		Direct3D->GetDeferredSRV(0), Direct3D->GetDeferredSRV(1), shadowMapShader->GetShadowSRV(),
		Direct3D->GetDeferredSRV(2), light->GetDirection(), lightVP, shadowMapShader->GetSize());

	lightShader->Draw(Direct3D->GetDeviceContext(), orthoWindow->GetIndexCount());

	Direct3D->TurnZBufferON();

	Direct3D->EndScene();

	return result;
}

void Application::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	Direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);

	///*Uncomment to render from the view of the light*/
	//shadowLight->GetViewMatrix(viewMatrix);
	//shadowLight->GetProjectionMatrix(projectionMatrix);

	////////////////////////////////////////////////////////////////////////// Render to shadow map //////////////////////////////////////////////////////////////////////////
	shadowMapShader->UseShader(Direct3D->GetDeviceContext());

	XMMATRIX lightView, lightProj;

	XMFLOAT3 lightPos = shadowLight->GetPosition();
	//lightWorld = XMMatrixTranslation(lightPos.x, lightPos.y, lightPos.z);
	shadowLight->GetViewMatrix(lightView);
	shadowLight->GetProjectionMatrix(lightProj);

	XMMATRIX vp = lightView * lightProj;

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		XMMATRIX world;
		spheres[i]->GetWorldMatrix(world);
		XMMATRIX wvp = world * vp;
		shadowMapShader->SetBuffers(Direct3D->GetDeviceContext(), wvp, shadowLight->GetPosition());
		spheres[i]->Render(Direct3D->GetDeviceContext());
	}

	//////////////////////////////////////////////////////////////////// Render scene with deferred shading ////////////////////////////////////////////////////////////////////
	Direct3D->ActivateDeferredShading();

	//Turns culling OFF to show that the terrain shader has its own backface culling in the geometry shader
	Direct3D->TurnCullingOFF();

	//Render terrain
	terrainShader->UseShader(Direct3D->GetDeviceContext());
	terrainShader->SetBuffers(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, terrain->GetTextures(), camera->GetPosition());
	terrain->Render(Direct3D->GetDeviceContext());

	Direct3D->TurnCullingON();

	//Render objects
	modelShader->UseShader(Direct3D->GetDeviceContext());
	
	modelQuadtree->Render(Direct3D->GetDeviceContext(), modelShader, viewMatrix, projectionMatrix);

	XMMATRIX world;
	sphere->GetWorldMatrix(world);
	modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	sphere->Render(Direct3D->GetDeviceContext());

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		spheres[i]->GetWorldMatrix(world);
		modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
		spheres[i]->Render(Direct3D->GetDeviceContext());
	}


	//Render particles
	particleShader->UseShader(Direct3D->GetDeviceContext());
	particleShader->SetMatrices(Direct3D->GetDeviceContext(), XMMatrixTranslation(128 - 30, 1, 64 - 30), viewMatrix, projectionMatrix, camera->GetPosition());
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
	shadowMapShader = new ShaderShadowMap(Direct3D->GetDevice(), L"assets/shaders/ShadowMapVS.hlsl", 2048, 2048, 0.0001f);
}
