#include "Terrain.h"
#include <iostream>

using namespace DirectX;
using namespace std;

Terrain::Terrain(ID3D11Device* device, char* heightMapName, float normalizeFactor)
{
	indexBuffer = nullptr;
	vertexBuffer = nullptr;
	vertexCount = 0;
	indexCount = 0;

	bool result = true;

	//Load and normalize heightmap
	result = LoadHeightMap(heightMapName);
	if (!result)
		throw runtime_error("LoadHeightMap Error");

	NormalizeHeightMap(normalizeFactor);

	//Initialize the vertex and index buffer that holds the geometry for the terrain.
	InitializeBuffers(device);
}
Terrain::~Terrain()
{
	//Release the index buffer.
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	//Release the vertex buffer.
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}

	if (heightMap)
	{
		delete[] heightMap;
		heightMap = nullptr;
	}
}

void Terrain::Render(ID3D11DeviceContext* deviceContext)
{
	SetBuffers(deviceContext);
	deviceContext->DrawIndexed(vertexCount, 0,0);
}

int Terrain::GetIndexCount()
{
	return indexCount;
}

float Terrain::GetY(float x, float z)
{
	float returnValue = 0.0f;

	//Normalizing with bilinear interpolation
	if (x <= terrainWidth-2 && z <= terrainHeight-2 && x >= 0 + 1 && z >= 0 + 1)
	{
		int x1, x2, z1, z2;
		float q11, q12, q21, q22;
		x1 = (int)floor(x);
		x2 = (int)floor(x+1);
		z1 = (int)floor(z);
		z2 = (int)floor(z+1);

		q11 = GetHeightAt(x1, z1);
		q12 = GetHeightAt(x1, z2);
		q21 = GetHeightAt(x2, z1);
		q22 = GetHeightAt(x2, z2);

		returnValue = (1.0f / ((x2 - x1)*(z2 - z1)))*(q11*(x2 - x)*(z2 - z) +
			q21*(x - x1)*(z2 - z) +
			q12*(x2 - x)*(z - z1) +
			q22*(x - x1)*(z - z1));
	}
	return returnValue;
}

bool Terrain::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize;
	unsigned char* bitmapImage;
	unsigned char height;

	// Open the height map file in binary
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	//Read file and info headers
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//Allocate the memory needed for the heightmap 
	terrainWidth = bitmapInfoHeader.biWidth;
	terrainHeight = bitmapInfoHeader.biHeight;
	imageSize = terrainWidth * terrainHeight * 3;
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	//Read the file
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	//Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	//Create the structure to hold the height map data
	heightMap = new HeightMap[terrainWidth * terrainHeight];
	if (!heightMap)
	{
		return false;
	}

	//Initialize the position in the image data buffer
	int k = 0;
	int index = 0;

	//Read the image data into the height map
	for (int j = 0; j < terrainHeight; j++)
	{
		for (int i = 0; i < terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (terrainHeight * j) + i;

			heightMap[index].x = (float)i;
			heightMap[index].y = (float)height;
			heightMap[index].z = (float)j;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return true;

}

void Terrain::NormalizeHeightMap(float factor)
{
	for (int i = 0; i < terrainHeight; i++)
	{
		for (int j = 0; j < terrainWidth; j++)
		{
			heightMap[(terrainHeight * i) + j].y /= factor;
		}
	}
}

float Terrain::GetHeightAt(int x, int z)
{
	return heightMap[(terrainHeight * z) + x].y;
}

void Terrain::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;
	int index1, index2, index3, index4;

	vertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 12;
	indexCount = vertexCount;

	VertexPosCol* vertices = new VertexPosCol[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];

	int index = 0;

	// Load the vertex and index array with the terrain data.
	for (int j = 0; j < (terrainHeight - 1); j++)
	{
		for (int i = 0; i < (terrainWidth - 1); i++)
		{
			index1 = (terrainHeight * j) + i;          
			index2 = (terrainHeight * j) + (i + 1);      
			index3 = (terrainHeight * (j + 1)) + i;      
			index4 = (terrainHeight * (j + 1)) + (i + 1);

			// Upper left.
			vertices[index].position = XMFLOAT3(heightMap[index3].x, heightMap[index3].y, heightMap[index3].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(heightMap[index4].x, heightMap[index4].y, heightMap[index4].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(heightMap[index4].x, heightMap[index4].y, heightMap[index4].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper left.
			vertices[index].position = XMFLOAT3(heightMap[index3].x, heightMap[index3].y, heightMap[index3].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(heightMap[index4].x, heightMap[index4].y, heightMap[index4].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Upper right.
			vertices[index].position = XMFLOAT3(heightMap[index4].x, heightMap[index4].y, heightMap[index4].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = XMFLOAT3(heightMap[index2].x, heightMap[index2].y, heightMap[index2].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom right.
			vertices[index].position = XMFLOAT3(heightMap[index2].x, heightMap[index2].y, heightMap[index2].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// Bottom left.
			vertices[index].position = XMFLOAT3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosCol) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	// Set up the description of the index buffer.
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

	delete[] vertices;
	vertices = nullptr;
	delete[] indices;
	indices = nullptr;
}

void Terrain::SetBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT32 vertexSize = sizeof(VertexPosCol);
	UINT32 offset = 0;

	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);

	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}