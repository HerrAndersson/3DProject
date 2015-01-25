#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

class D3DClass
{

private:

	IDXGISwapChain*				swapChain;
	ID3D11Device*			    device;
	ID3D11DeviceContext*		deviceContext;
	ID3D11RenderTargetView*		renderTargetView;

	//Used for 2D rendering, ex. text.
	ID3D11Texture2D*			depthStencilBuffer;
	ID3D11DepthStencilState*	depthStencilState;
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11RasterizerState*		rasterState;
	ID3D11DepthStencilState*    depthDisabledStencilState;
	ID3D11BlendState*			alphaEnableBlendingState;
	ID3D11BlendState*			alphaDisableBlendingState;

	XMMATRIX projectionMatrix;
	XMMATRIX worldMatrix;
	XMMATRIX orthoMatrix;

public:

	D3DClass();
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX& projectionMatrix);
	void GetWorldMatrix(XMMATRIX& worldMatrix);
	void GetOrthoMatrix(XMMATRIX& orthoMatrix);

};

