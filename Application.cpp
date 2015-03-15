#include "Application.h"
#include <iostream>
#include <DirectXCollision.h>

using namespace DirectX;
using namespace std;

Application::Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	Direct3D = new D3DClass(screenWidth, screenHeight, hwnd, false, 1000, 0.1f);
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

	spheres = new ObjectIntersection(Direct3D->GetDevice(), "assets/models/ball.obj", "assets/textures/missing.raw", XMFLOAT3(128, 5, 128), XMFLOAT3(2, 2, 2), tempWorldMatrix);

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
		delete [] spheres;
		spheres = nullptr;
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

	((ObjectIntersection*)spheres)->Update();

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
		bool intersect = TestIntersections((ObjectIntersection*)spheres);
		if (intersect)
		{
			cout << "INTERSECT" << endl;
			system("cls");

			XMFLOAT3 p = ((ObjectIntersection*)spheres)->GetPosition();
			((ObjectIntersection*)spheres)->SetPosition(XMFLOAT3(p.x, p.y + 10, p.z));
		}
	}
}

bool Application::TestIntersections(ObjectIntersection* object)
{
	bool intersect = false;

	XMMATRIX p;
	Direct3D->GetProjectionMatrix(p);
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, p);

	XMFLOAT2 mp = input->GetMouseLocation();

	float vx = (+2.0f * mp.x / screenWidth - 1.0f) / m._11;
	float vy = (-2.0f * mp.y / screenHeight + 1.0f) / m._22;

	XMVECTOR rayOrigin{ { 0.0f, 0.0f, 0.0f } };
	XMVECTOR rayDir{ { vx, vy, 1.0f } };

	XMMATRIX v;
	camera->GetViewMatrix(v);
	v = XMMatrixInverse(&XMMatrixDeterminant(v), v);

	rayOrigin = XMVector3TransformCoord(rayOrigin, v);
	rayDir = XMVector3TransformNormal(rayDir, v);

	XMMATRIX w;
	object->GetWorldMatrix(w);
	XMMATRIX w2;
	Direct3D->GetWorldMatrix(w2);

	w = XMMatrixMultiply(w, w2);

	w = XMMatrixInverse(&XMMatrixDeterminant(w), w);

	rayOrigin = XMVector3TransformCoord(rayOrigin, w);
	rayDir = XMVector3TransformNormal(rayDir, w);

	rayDir = XMVector3Normalize(rayDir);

	XMFLOAT3 ro;
	XMFLOAT3 rd;
	XMStoreFloat3(&ro, rayOrigin);
	XMStoreFloat3(&rd, rayDir);

	intersect = RaySphereIntersect(ro, rd, 2.0f);
	Ray r(ro, rd);
	//intersect = RayVsSphere(r, *object->GetIntersectionSphere());
	//intersect = RayVsSphere2(r, *object->GetIntersectionSphere());
	return intersect;


	///////////////////////////////////////////////////////////////// Funkar inuti sphere, men inte utanför //////////////////////////////////////////////////
	//XMMATRIX p;
	//Direct3D->GetProjectionMatrix(p);

	//XMFLOAT4X4 m;
	//XMStoreFloat4x4(&m, p);

	//XMFLOAT2 mousePos = input->GetMouseLocation();

	//// Compute picking ray in view space.
	//float vx = (+2.0f*mousePos.x / screenWidth - 1.0f) / m._11;
	//float vy = (-2.0f*mousePos.y / screenHeight + 1.0f) / m._22;

	//// Ray definition in view space.
	//XMFLOAT3 campos = camera->GetPosition();
	//XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	//// Tranform ray to local space of Mesh.
	//XMMATRIX v;
	//camera->GetViewMatrix(v);
	//XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(v), v);

	//XMMATRIX w;
	//object->GetWorldMatrix(w);
	//XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(w), w);

	//XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	//rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	//rayDir = XMVector3TransformNormal(rayDir, toLocal);

	//// Make the ray direction unit length for the intersection tests.
	//rayDir = XMVector3Normalize(rayDir);

	//XMFLOAT3 ro;
	//XMFLOAT3 rd;
	//XMStoreFloat3(&ro, rayOrigin);
	//XMStoreFloat3(&rd, rayDir);

	////intersect = RaySphereIntersect(ro, rd, 2.0f);

	//return intersect;







	//// Move the mouse cursor coordinates into the -1 to +1 range.
	//XMFLOAT2 mousePos = input->GetMouseLocation();
	//float pointX = (((2.0f * (float)mousePos.x) / (float)screenWidth) - 1.0f);
	//float pointY = -(((2.0f * (float)mousePos.y) / (float)screenHeight) - 1.0f);

	//XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, inverseWorldMatrix;
	//XMFLOAT3 direction, origin;

	//camera->GetViewMatrix(viewMatrix);
	//Direct3D->GetProjectionMatrix(projectionMatrix);

	////Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	//XMFLOAT4X4 m2;
	//XMStoreFloat4x4(&m2, projectionMatrix);

	//pointX = pointX / m2._11;
	//pointY = pointY / m2._22;

	//// Get the inverse of the view matrix.
	//XMFLOAT4X4 m;
	//inverseViewMatrix = XMMatrixInverse(&XMMatrixDeterminant(viewMatrix), viewMatrix);
	//XMStoreFloat4x4(&m, inverseViewMatrix);

	//// Calculate the direction of the picking ray in view space.
	//direction.x = (pointX * m._11) + (pointY * m._21) + m._31;
	//direction.y = (pointX * m._12) + (pointY * m._22) + m._32;
	//direction.z = (pointX * m._13) + (pointY * m._23) + m._33;

	//// Get the origin of the picking ray which is the position of the camera.
	//origin = camera->GetPosition();

	//// Get the world matrix and translate to the location of the sphere.
	//Direct3D->GetWorldMatrix(worldMatrix);
	//XMFLOAT3 pos = object->GetIntersectionSphere()->center;

	////translateMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);
	//XMMATRIX wm;
	//object->GetWorldMatrix(wm);
	//worldMatrix = XMMatrixMultiply(worldMatrix, wm);

	//// Now get the inverse of the translated world matrix.
	//inverseWorldMatrix = XMMatrixInverse(&XMMatrixDeterminant(worldMatrix), worldMatrix);

	//XMFLOAT3 rayOrigin, rayDir;
	//// Now transform the ray origin and the ray direction from view space to world space.
	//XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMLoadFloat3(&origin), inverseWorldMatrix));
	//XMStoreFloat3(&rayDir, XMVector3TransformNormal(XMLoadFloat3(&direction), inverseWorldMatrix));

	//// Normalize the ray direction.
	//XMStoreFloat3(&rayDir, XMVector3Normalize(XMLoadFloat3(&rayDir)));

	//// Now perform the ray-sphere intersection test.
	//intersect = RaySphereIntersect(rayOrigin, rayDir, object->GetIntersectionSphere()->radius);

	////Ray r(rayOrigin, rayDir);
	////intersect = RayVsSphere(r, *object->GetIntersectionSphere());
	//return intersect;

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


	XMFLOAT3 center = ((ObjectIntersection*)spheres)->GetIntersectionSphere()->center;
	float radius = ((ObjectIntersection*)spheres)->GetIntersectionSphere()->radius;
	world = XMMatrixScaling(radius, radius, radius)*XMMatrixTranslation(center.x, center.y, center.z);
	modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	((ObjectIntersection*)spheres)->RenderSphere(Direct3D->GetDeviceContext());

	spheres->GetWorldMatrix(world);
	modelShader->SetMatrices(Direct3D->GetDeviceContext(), world, viewMatrix, projectionMatrix);
	spheres->Render(Direct3D->GetDeviceContext());

	modelQuadtree->Render(Direct3D->GetDeviceContext(), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));

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
