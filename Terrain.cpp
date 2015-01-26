#include "Terrain.h"

Terrain::Terrain()
{
	indexBuffer = nullptr;
	vertexBuffer = nullptr;
	vertexCount = 0;
	indexCount = 0;
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

bool Terrain::Initialize(ID3D11Device* device)
{
	bool result = true;

	terrainWidth = 100;
	terrainHeight = 100;

	// Initialize the vertex and index buffer that holds the geometry for the terrain.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return result;
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

bool Terrain::InitializeBuffers(ID3D11Device* device)
{
	return true;
}

void Terrain::SetBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT32 vertexSize = sizeof(VertexPosCol);
	UINT32 offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//deviceContext->IASetInputLayout(vertexLayout);
}