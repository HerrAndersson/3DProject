#include "OrthoWindow.h"

OrthoWindow::OrthoWindow(ID3D11Device* device, int windowWidth, int windowHeight)
{
	vertexCount = 0;
	indexCount = 0;

	InitializeBuffers(device, windowWidth, windowHeight);
}

OrthoWindow::~OrthoWindow()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}

	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}

void OrthoWindow::InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Screen coordinates of the window
	float left = -1.0f;
	float right = 1.0f;
	float top = 1.0f;
	float bottom = -1.0f;
	

	vertexCount = 6;
	indexCount = vertexCount;

	VertexPosUV* vertices = new VertexPosUV[vertexCount];
	unsigned long* indices = new unsigned long[indexCount]; 

	// Load the vertex array with data
	// First triangle
	vertices[0].pos = XMFLOAT3(left, top, 0.0f);			 // Top left.
	vertices[0].uv = XMFLOAT2(0.0f, 0.0f);

	vertices[1].pos = XMFLOAT3(right, bottom, 0.0f);		// Bottom right.
	vertices[1].uv = XMFLOAT2(1.0f, 1.0f);

	vertices[2].pos = XMFLOAT3(left, bottom, 0.0f);			// Bottom left.
	vertices[2].uv = XMFLOAT2(0.0f, 1.0f);
	// Second triangle
	vertices[3].pos = XMFLOAT3(left, top, 0.0f);			// Top left.
	vertices[3].uv = XMFLOAT2(0.0f, 0.0f);

	vertices[4].pos = XMFLOAT3(right, top, 0.0f);			// Top right.
	vertices[4].uv = XMFLOAT2(1.0f, 0.0f);

	vertices[5].pos = XMFLOAT3(right, bottom, 0.0f);		// Bottom right.
	vertices[5].uv = XMFLOAT2(1.0f, 1.0f);

	//Load the index array with data
	for (int i = 0; i < indexCount; i++)
	{
		indices[i] = i;
	}

	//Description of the vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosUV) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		throw runtime_error("Error creating vertex buffer in OrthoWindow");
	}

	//Description of the index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		throw runtime_error("Error creating index buffer in OrthoWindow");
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;
}

void OrthoWindow::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexPosUV);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int OrthoWindow::GetIndexCount()
{
	return indexCount;
}
