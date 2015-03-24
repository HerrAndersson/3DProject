#pragma once
#include "ShaderBase.h"
using namespace DirectX;
class ShaderLight : public ShaderBase
{

private:

	//struct MatrixBuffer
	//{
	//	XMMATRIX world;
	//	XMMATRIX view;
	//	XMMATRIX projection;
	//	XMMATRIX lightVP;
	//};

	struct LightBuffer
	{
		DirectX::XMMATRIX lightVP;
		DirectX::XMFLOAT3 lightDirection;
		int size;
	};

	ID3D11SamplerState*		sampleState;
	ID3D11Buffer*			matrixBuffer;
	ID3D11Buffer*			lightBuffer;

	//void SetMatrixBuffer(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, XMMATRIX& lightVP);
	void SetLightBuffer(ID3D11DeviceContext* deviceContext, DirectX::XMFLOAT3 lightDirection, int shadowMapSize, XMMATRIX& lightVP);

public:

	ShaderLight(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename);
	virtual ~ShaderLight();

	void SetBuffers(ID3D11DeviceContext* deviceContext, 
		ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* shadowTexture, ID3D11ShaderResourceView* worldPosTexture, 
		DirectX::XMFLOAT3 lightDirection, XMMATRIX& lightWVP, int shadowMapSize);

	virtual void Draw(ID3D11DeviceContext* deviceContext, int indexCount);

};

