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

	spheres = new ObjectBase*[NUM_SPHERES];

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		XMFLOAT3 pos((float)(150 + (i * 20 + 15 * i)), 20 + i * 5, 170);
		XMFLOAT3 scale((float)(4 + i*i), (float)(4 + i*i), (float)(4 + i*i));
		spheres[i] = new ObjectIntersection(Direct3D->GetDevice(), "assets/models/sphere3.obj", pos, scale, XMMatrixIdentity());
	}

	XMMATRIX tempWorldMatrix = XMMatrixIdentity();
	sphere = new ObjectIntersection(Direct3D->GetDevice(), "assets/models/sphere3.obj", XMFLOAT3(350, 15, 128), XMFLOAT3(5, 5, 5), tempWorldMatrix);

	// Initialize the light object.
	XMFLOAT4 ambient(0.05f, 0.05f, 0.05f, 1.0f);
	XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT3 position(256.0f, 256.0f, -20.0f);
	XMFLOAT3 lookAt(256.0f, 0.0f, 256.0f);

	light = new Light(ambient, diffuse, position, lookAt, screenDepth, screenNear);
	shadowMap = new ShadowMap(Direct3D->GetDevice(), 2048, L"assets/shaders/ShaderShadowVS.hlsl");

	CreateShaders();
}

Application::~Application()
{
	delete Direct3D;
	delete camera;
	delete terrain;
	delete timer;
	delete input;
	delete position;
	delete light;
	delete shadowMap;

	//MODELS
	if (spheres)
	{
		for (int i = 0; i < NUM_SPHERES; i++)
		{
			delete spheres[i];
		}
		delete[] spheres;
	}
	delete sphere;

	//SHADERS
	delete modelShader;
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

	for (int i = 0; i < NUM_SPHERES; i++)
		((ObjectIntersection*)spheres[i])->Update();

	((ObjectIntersection*)sphere)->Update();

	result = Render();

	//Check if the user pressed escape and wants to exit the application.
	if (input->Escape()) { result = false; }

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

bool Application::Render()
{
	bool result = true;

	ID3D11DeviceContext* deviceContext = Direct3D->GetDeviceContext();
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/////////////////////////////////////////////////////////////////////// RENDER SHADOW MAP /////////////////////////////////////////////////////////////////////// 
	XMMATRIX modelWorld, lightView, lightProj, wvp;
	light->GetProjectionMatrix(lightProj);
	light->GetViewMatrix(lightView);

	shadowMap->ActivateShadowRendering(deviceContext);

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		spheres[i]->GetWorldMatrix(modelWorld);
		wvp = modelWorld * lightView * lightProj;
		shadowMap->SetBuffer(deviceContext, wvp);
		spheres[i]->Render(deviceContext);
	}

	///////////////////////////////////////////////////////////////////////// RENDER SCENE ////////////////////////////////////////////////////////////////////////// 
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightVP;
	Direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	lightVP = lightView * lightProj;

	Direct3D->BeginScene(0.2f, 0.4f, 1.0f, 1.0f);

	modelShader->UseShader(deviceContext);

	ID3D11ShaderResourceView* shadows = shadowMap->GetShadowSRV();
	deviceContext->PSSetShaderResources(1, 1, &shadows);

	deviceContext->PSSetShaderResources(0, 1, &terrain->GetTextures()[0]);
	modelShader->SetBuffers(deviceContext, worldMatrix, viewMatrix, projectionMatrix, lightVP);
	terrain->Render(deviceContext);

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		spheres[i]->GetWorldMatrix(worldMatrix);
		modelShader->SetBuffers(deviceContext, worldMatrix, viewMatrix, projectionMatrix, lightVP);
		spheres[i]->Render(deviceContext);
	}

	Direct3D->EndScene();

	return result;
}

void Application::CreateShaders()
{
	modelShader = new ShaderDefault(Direct3D->GetDevice(), L"assets/shaders/ShaderSceneVS.hlsl", L"assets/shaders/ShaderScenePS.hlsl");
}
