#pragma once
#include "ShaderBase.h"

using namespace DirectX;
using namespace std;

class ShaderLight : public ShaderBase
{

private:

	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBuffer
	{
		XMFLOAT3 lightDirection;
		float padding;
	};

	ID3D11SamplerState*		sampleState;
	ID3D11Buffer*			matrixBuffer;
	ID3D11Buffer*			lightBuffer;

	void SetMatrixBuffer(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	void SetLightBuffer(ID3D11DeviceContext* deviceContext, XMFLOAT3 lightDirection);

public:

	ShaderLight(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename);
	virtual ~ShaderLight();

	void SetBuffers(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* colorTexture,
					ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDirection);

	virtual void Draw(ID3D11DeviceContext* deviceContext, int indexCount);

};

