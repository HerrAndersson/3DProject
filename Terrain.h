#ifndef _TERRAIN_H_
#define _TERRAIN_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>

using namespace DirectX;

class Terrain
{
private:

	struct VertexType
	{
		XMVECTOR position;
		XMVECTOR color;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	int terrainWidth;
	int terrainHeight;
	int vertexCount;
	int indexCount;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

public:

	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
};

#endif