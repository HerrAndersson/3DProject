#include "Application.h"

using namespace DirectX;
using namespace std;

Application::Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	Direct3D = nullptr;
	defaultShader = nullptr;
	camera = nullptr;
	terrain = nullptr;



	bool result = true;
	XMMATRIX baseViewMatrix;

	Direct3D = new D3DClass(screenWidth, screenHeight, hwnd, false, 1000, 0.5);

	camera = new Camera();

	camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -1.0f));
	camera->Update();
	camera->GetViewMatrix(baseViewMatrix);
	camera->SetPosition(XMFLOAT3(0.0f, 20.0f, -10.0f));
	camera->SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));

	terrain = new Terrain(Direct3D->GetDevice());

	CreateShaders(hwnd);
	CreateTriangleData();
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

	if (!result)
	{
		return false;
	}

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

	Direct3D->BeginScene(0.1f, 0.2f, 0.4f, 1.0f);

	camera->Update();
	camera->GetViewMatrix(viewMatrix);

	Direct3D->GetWorldMatrix(worldMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	terrainShader->UseShader(Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	terrain->Render(Direct3D->GetDeviceContext());

	if (!result)
	{
		return false;
	}

	defaultShader->UseShader(Direct3D->GetDeviceContext(), vertexBuffer);
	Direct3D->GetDeviceContext()->Draw(100, 0);

	Direct3D->EndScene();

	return result;
}

void Application::CreateTriangleData()
{
	struct TriangleVertex
	{
		float x, y, z;
		float r, g, b;
	}
	triangleVertices[6] =
	{
		0.0f, 0.5f, 0.0f,	//v0 pos
		1.0f, 0.0f, 0.0f,	//v0 color

		0.5f, -0.5f, 0.0f,	//v1
		0.0f, 1.0f, 0.0f,	//v1 color

		-0.5f, -0.5f, 0.0f, //v2
		0.0f, 0.0f, 1.0f,	//v2 color

	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	Direct3D->GetDevice()->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
}


bool Application::CreateShaders(HWND hwnd)
{
	bool result = true;
	//Create Default shader object.
	defaultShader = new ShaderBase();

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
	{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Initialize the Default shader object.
	result = defaultShader->Initialize(Direct3D->GetDevice(), hwnd, inputDesc, ARRAYSIZE(inputDesc), L"assets/shaders/VertexShader.hlsl", L"assets/shaders/PixelShader.hlsl");
	if (!result)
	{
		throw runtime_error("Could not initialize Default shader.");
	}

	terrainShader = new ShaderColor();

	result = terrainShader->Initialize(Direct3D->GetDevice(), hwnd, L"assets/shaders/vsTerrain.hlsl", L"assets/shaders/PixelShader.hlsl");
	if (!result)
	{
		throw runtime_error("Could not initialize Terrainshader.");
	}
}
