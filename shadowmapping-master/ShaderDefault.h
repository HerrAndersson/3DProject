#pragma once
#include "ShaderBase.h"
using namespace DirectX;
class ShaderDefault : public ShaderBase
{

private:

	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightVP;
	};

	ID3D11Buffer* matrixBuffer;

	ID3D11SamplerState* samplerState;

public:

	ShaderDefault(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename);
	virtual ~ShaderDefault();

	virtual void UseShader(ID3D11DeviceContext* deviceContext);

	void SetBuffers(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix, XMMATRIX& lightVP);

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);
};

