#pragma once
#include "ShaderBase.h"

class ShaderDeferred : public ShaderBase
{

private:

	static const int BUFFER_COUNT = 2;

	//__declspec(align(16))
	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	int textureWidth;
	int textureHeight;

	ID3D11Texture2D*			 renderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView*		 renderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView*	 shaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D*			 depthStencilBuffer;
	ID3D11DepthStencilView*		 depthStencilView;
	D3D11_VIEWPORT				 viewport;

	ID3D11SamplerState*			 sampleStateWrap;
	ID3D11Buffer*				 matrixBuffer;

public:

	ShaderDeferred(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename, int textureWidth, int textureHeight);
	virtual ~ShaderDeferred();

	void InitializeBuffers(ID3D11Device* device);
	void InitializeShaderData(ID3D11Device* device);

	void SetRenderTargets(ID3D11DeviceContext* deviceContext);
	void ClearRenderTargets(ID3D11DeviceContext* deviceContext, float r, float g, float b, float a);

	ID3D11ShaderResourceView* GetShaderResourceView(int viewNumber);

	virtual void UseShader(ID3D11DeviceContext* deviceContext);
	void SetBuffers(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture);

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);

};
