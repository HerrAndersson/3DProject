#include "Application.h"


Application::Application()
{
	Direct3D = nullptr;
	defaultShader = nullptr;
}

Application::~Application()
{
	// Release the Direct3D object.
	if (Direct3D)
	{
		delete Direct3D;
		Direct3D = nullptr;
	}

	if (defaultShader)
	{
		delete defaultShader;
		defaultShader = nullptr;
	}
}

bool Application::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result = true;
	float cameraX, cameraY, cameraZ;
	XMMATRIX baseViewMatrix;

	//Create the Direct3D object.
	Direct3D = new D3DClass();
	if (!Direct3D)
	{
		return false;
	}

	//Initialize the Direct3D object.
	result = Direct3D->Initialize(screenWidth, screenHeight, hwnd, false, 1000, 0.5);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	CreateShaders(hwnd);
	CreateTriangleData();

	//If it reaches this point it means no errors happened
	return true;
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

	//Camera->Update();
	//Camera->GetViewMatrix(viewMatrix);

	Direct3D->GetWorldMatrix(worldMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	// Render the terrain buffers.
	//Terrain->Render(Direct3D->GetDeviceContext());

	// Render the terrain using the terrain shader.
	//result = TerrainShader->Render(Direct3D->GetDeviceContext(), Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	Light->GetAmbientColor(), Light->GetDiffuseColor(), Light->GetDirection(), Terrain->GetTexture());
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
	bool result;
	//Create Default shader object.
	defaultShader = new ShaderBase();
	if (!defaultShader)
	{
		return false;
	}

	//Initialize the Default shader object.
	result = defaultShader->Initialize(Direct3D->GetDevice(), L"assets/shaders/VertexShader.hlsl", L"assets/shaders/PixelShader.hlsl", hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Default shader.", L"Error", MB_OK);
		return false;
	}

	return true;
}
