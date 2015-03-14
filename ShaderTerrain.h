#pragma once
#include "ShaderBase.h"
#include <stdexcept>
#include "Light.h"
#include "BufferTypes.h"

using namespace DirectX;

class ShaderTerrain : public ShaderBase
{

private:

	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX wvp;
	};

	struct LightBuffer
	{
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT3 lightDirection;
		float padding;
	};

	struct GSBuffer
	{
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMFLOAT3 camPos;
		float padding;
	};

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* gsBuffer;
	//ID3D11Buffer* lightBuffer;
	ID3D11SamplerState* samplerState;

public:

	ShaderTerrain(ID3D11Device* device, LPCWSTR vsFilename, LPCWSTR psFilename, LPCWSTR gsFilename);
	virtual ~ShaderTerrain();

	virtual void UseShader(ID3D11DeviceContext* deviceContext);

	void SetBuffers(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, 
					XMMATRIX& projectionMatrix, ID3D11ShaderResourceView** textures, XMFLOAT3 camPos);

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);
};

