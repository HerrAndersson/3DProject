#ifndef _TERRAIN_H_
#define _TERRAIN_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>
#include "VertexTypes.h"

using namespace DirectX;

class Terrain
{
private:

	int terrainWidth;
	int terrainHeight;
	int vertexCount;
	int indexCount;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	bool InitializeBuffers(ID3D11Device* device);
	void SetBuffers(ID3D11DeviceContext* deviceContext);

public:

	Terrain();
	~Terrain();

	bool Initialize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
};

#endif