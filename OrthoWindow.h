#pragma once
#include "VertexTypes.h"
#include <Windows.h>
#include <d3d11.h>
#include <stdexcept>

class OrthoWindow
{
private:

	int vertexCount;
	int indexCount;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	void InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight);

public:

	OrthoWindow(ID3D11Device* device, int windowWidth, int windowHeight);
	~OrthoWindow();

	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
};

