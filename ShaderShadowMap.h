#pragma once
#include "ShaderBase.h"
#include "ShadowMap.h"

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

	ID3D11RasterizerState*		 rasterizerState;
	ID3D11DepthStencilState*	 depthStencilState;

	ID3D11VertexShader*			 vertexShader;
	ID3D11Buffer*				 matrixBuffer;
	ID3D11InputLayout*			 inputLayout;

public:

	ShaderShadowMap(ID3D11Device* device, LPCWSTR vertexShaderFilename);
	virtual ~ShaderShadowMap();

	void UseShader(ID3D11DeviceContext* deviceContext);
	void SetBuffers(ID3D11DeviceContext* deviceContext, XMMATRIX& lightWVP, XMFLOAT3 lightPos);

	void* operator new(size_t i);
	void operator delete(void* p);
};

