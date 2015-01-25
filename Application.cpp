#include "Application.h"


Application::Application()
{
	Direct3D = nullptr;
}

Application::~Application()
{

	// Release the Direct3D object.
	if (Direct3D)
	{
		delete Direct3D;
		Direct3D = nullptr;
	}
}

bool Application::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result = true;
	float cameraX, cameraY, cameraZ;
	XMMATRIX baseViewMatrix;

	// Create the Direct3D object.
	Direct3D = new D3DClass();
	if (!Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = Direct3D->Initialize(screenWidth, screenHeight, hwnd, false, 20, 0.5);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	CreateShaders();
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
	//	Light->GetAmbientColor(), Light->GetDiffuseColor(), m_Light->GetDirection(), Terrain->GetTexture());
	if (!result)
	{
		return false;
	}

	Direct3D->GetDeviceContext()->VSSetShader(defaultVertexShader, nullptr, 0);
	Direct3D->GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	Direct3D->GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	Direct3D->GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	Direct3D->GetDeviceContext()->PSSetShader(defaultPixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(float) * 6;
	UINT32 offset = 0;
	Direct3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	Direct3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Direct3D->GetDeviceContext()->IASetInputLayout(defaultVertexLayout);

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


void Application::CreateShaders()
{
	//create vertex shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(L"assets/shaders/VertexShader.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", NULL, NULL, &pVS, NULL);
	Direct3D->GetDevice()->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &defaultVertexShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Direct3D->GetDevice()->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &defaultVertexLayout);
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(L"assets/shaders/PixelShader.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", NULL, NULL, &pPS, NULL);
	Direct3D->GetDevice()->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &defaultPixelShader);
	pPS->Release();
}
