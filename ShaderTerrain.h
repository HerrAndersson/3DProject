#pragma once
#include "ShaderBase.h"
#include <stdexcept>
#include "Light.h"
#include "BufferTypes.h"

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

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11SamplerState* samplerState;

public:

	ShaderTerrain(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename);
	virtual ~ShaderTerrain();

	virtual void UseShader(ID3D11DeviceContext* deviceContext);

	void SetBuffers(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix,
					Light* light, float padding, ID3D11ShaderResourceView** textures);

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);
};

