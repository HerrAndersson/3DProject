#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <stdexcept>
#include "Deferred.h"
#include "ShadowMap.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class D3DClass
{

private:

	IDXGISwapChain*				swapChain;
	ID3D11Device*			    device;
	ID3D11DeviceContext*		deviceContext;
	ID3D11RenderTargetView*		renderTargetView;
	D3D11_VIEWPORT				viewport;
	Deferred*                   deferredShader;

	ShadowMap*                  shadowMap;

	ID3D11DepthStencilView*     depthStencilView;
	ID3D11Texture2D*			depthStencilBuffer;
	ID3D11RasterizerState*		rasterState;
	ID3D11RasterizerState*      rasterNoCullingState;
	ID3D11DepthStencilState*	depthStencilState;
	ID3D11DepthStencilState*    depthDisabledStencilState;
	ID3D11BlendState*			alphaEnableBlendingState;
	ID3D11BlendState*			alphaDisableBlendingState;

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;

public:

	D3DClass(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	~D3DClass();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix);
	void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix);
	void GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix);

	ID3D11ShaderResourceView* GetDeferredSRV(int viewNumber);

	ID3D11ShaderResourceView* GetShadowMapSRV();
	int GetShadowMapSize();

	void ActivateDeferredShading();
	void ActivateShadowing();

	void TurnZBufferON();
	void TurnZBufferOFF();
	void TurnAlphaBlendingON();
	void TurnAlphaBlendingOFF();
	void TurnCullingON();
	void TurnCullingOFF();

	void SetBackBufferRenderTarget();
	void ResetViewport();

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);

};

