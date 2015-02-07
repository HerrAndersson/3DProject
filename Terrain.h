#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdexcept>
#include <windows.h>
#include "VertexTypes.h"

class Terrain
{
private:

	int terrainWidth;
	int terrainHeight;
	int vertexCount;
	int indexCount;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	void InitializeBuffers(ID3D11Device* device);
	void SetBuffers(ID3D11DeviceContext* deviceContext);

public:

	Terrain(ID3D11Device* device);
	~Terrain();

	void Render(ID3D11DeviceContext* deviceContext);

	int GetY(int x, int z);

	int GetIndexCount();
};
