#pragma once
#include "ShaderBase.h"

class ShaderLight : public ShaderBase
{

private:

	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	struct LightBuffer
	{
		DirectX::XMFLOAT3 lightDirection;
		float padding;
	};

	ID3D11SamplerState*		sampleState;
	ID3D11Buffer*			matrixBuffer;
	ID3D11Buffer*			lightBuffer;

	void SetMatrixBuffer(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix);
	void SetLightBuffer(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT3 lightDirection);

public:

	ShaderLight(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename);
	virtual ~ShaderLight();

	void SetBuffers(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* colorTexture,
		ID3D11ShaderResourceView* normalTexture, DirectX::XMFLOAT3 lightDirection);

	virtual void Draw(ID3D11DeviceContext* deviceContext, int indexCount);

};

