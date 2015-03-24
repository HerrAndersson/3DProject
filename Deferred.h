#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <d3dcompiler.h>
#include <string>
#include <fstream>

class Deferred
{

private:

	//Diffuse, Normal and WorldPosition (used the shadow mapping)
	static const int BUFFER_COUNT = 3;

	int textureWidth;
	int textureHeight;

	ID3D11Texture2D*			 renderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView*		 renderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView*	 shaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D*			 depthStencilBuffer;
	ID3D11DepthStencilView*		 depthStencilView;
	D3D11_VIEWPORT				 viewport;

	void InitializeBuffers(ID3D11Device* device);

public:

	Deferred(ID3D11Device* device, int textureWidth, int textureHeight);
	virtual ~Deferred();

	void SetRenderTargets(ID3D11DeviceContext* deviceContext);
	void ClearRenderTargets(ID3D11DeviceContext* deviceContext, float r, float g, float b, float a);

	ID3D11ShaderResourceView* GetShaderResourceView(int viewNumber);

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);

};
