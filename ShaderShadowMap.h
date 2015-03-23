#pragma once
#include "ShaderBase.h"

using namespace DirectX;
class ShaderShadowMap
{
private:

	struct MatrixBuffer
	{
		XMMATRIX lightWVP;
		XMFLOAT3 lightPos;
		float padding;
	};

	int width, height;

	ID3D11Texture2D*			 shadowMap;
	ID3D11DepthStencilView*		 shadowMapDepthView;
	ID3D11ShaderResourceView*	 shadowMapSRV;
	D3D11_VIEWPORT				 shadowMapViewport;
	float						 shadowMapBias;

	ID3D11RasterizerState*		 rasterizerState;
	ID3D11DepthStencilState*	 depthStencilState;

	ID3D11VertexShader*			 vertexShader;
	ID3D11Buffer*				 matrixBuffer;
	ID3D11InputLayout*			 inputLayout;

public:

	ShaderShadowMap(ID3D11Device* device, LPCWSTR vertexShaderFilename, int width, int height, float bias);
	virtual ~ShaderShadowMap();

	void UseShader(ID3D11DeviceContext* deviceContext);
	void SetBuffers(ID3D11DeviceContext* deviceContext, XMMATRIX& lightWVP, XMFLOAT3 lightPos);

	ID3D11ShaderResourceView* GetShadowSRV();

	int GetSize();

	void* operator new(size_t i);
	void operator delete(void* p);
};

