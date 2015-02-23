#pragma once
#include "ShaderBase.h"
#include "BufferTypes.h"

class ShaderDefault : public ShaderBase
{

private:

	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX wvp;
	};

	ID3D11Buffer* matrixBuffer;

public:

	ShaderDefault(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename);
	virtual ~ShaderDefault();

	virtual void UseShader(ID3D11DeviceContext* deviceContext);

	void SetMatrices(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix);
	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);
};

