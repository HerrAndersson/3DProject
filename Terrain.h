#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdexcept>
#include <windows.h>
#include "VertexTypes.h"

class Terrain
{
private:

	struct HeightMap
	{
		float x, y, z;
	};

	int terrainWidth;
	int terrainHeight;
	int vertexCount;
	int indexCount;

	HeightMap* heightMap;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	void InitializeBuffers(ID3D11Device* device);
	void SetBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadHeightMap(char* filename);
	void NormalizeHeightMap(float factor);
	float GetHeightAt(int x, int z);

public:

	Terrain(ID3D11Device* device, char* heightMapName, float normalizeFactor);
	~Terrain();

	void Render(ID3D11DeviceContext* deviceContext);

	float GetY(float x, float z);
	

	int GetIndexCount();
};
