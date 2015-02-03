#include "Terrain.h"

using namespace DirectX;
using namespace std;

Terrain::Terrain(ID3D11Device* device)
{
	indexBuffer = nullptr;
	vertexBuffer = nullptr;
	vertexCount = 0;
	indexCount = 0;


	bool result = true;

	terrainWidth = 100;
	terrainHeight = 100;
	// Initialize the vertex and index buffer that holds the geometry for the terrain.
	InitializeBuffers(device);
}
Terrain::~Terrain()
{
	// Release the index buffer.
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

void Terrain::Render(ID3D11DeviceContext* deviceContext)
{
	SetBuffers(deviceContext);
	deviceContext->Draw(vertexCount, 0);
}

int Terrain::GetIndexCount()
{
	return indexCount;
}

void Terrain::InitializeBuffers(ID3D11Device* device)
{
	VertexPosCol* vertices = nullptr;
	unsigned long* indices = nullptr;
	int index = 0;
	float positionX, positionZ;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 8;

	indexCount = vertexCount;

	vertices = new VertexPosCol[vertexCount];

	indices = new unsigned long[indexCount];

	//Create the terrain data
	for (int j = 0; j < (terrainHeight - 1); j++)
	{
		for (int i = 0; i < (terrainWidth - 1); i++)
		{
			// LINE 1
			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// LINE 2
			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// LINE 3
			// Bottom right.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			positionX = (float)i;
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// LINE 4
			// Bottom left.
			positionX = (float)i;
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;
		}
	}

	//Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosCol) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		throw runtime_error("Could not create terrain vertex buffer");
	}

	//Set up the description of the index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		throw runtime_error("Could not create terrain index buffer");
	}

	//Release the arrays when the buffers have all data
	delete[] vertices;
	delete[] indices;
}

void Terrain::SetBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT32 vertexSize = sizeof(VertexPosCol);
	UINT32 offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceContext->IASetInputLayout(vertexLayout);
}