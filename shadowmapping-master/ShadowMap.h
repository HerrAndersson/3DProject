#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <iostream>
#include <d3dcompiler.h>
#include <string>

using namespace DirectX;
using namespace std;

class ShadowMap
{
private:

	struct MatrixBuffer
	{
		XMMATRIX wvp;
	};

	float dimensions;

	ID3D11Texture2D*				shadowMap;
	ID3D11DepthStencilView*			shadowDepthView;
	ID3D11ShaderResourceView*		shadowResourceView;

	ID3D11RasterizerState*          shadowRasterState;
	ID3D11DepthStencilState*        shadowDepthState;

	D3D11_VIEWPORT					shadowViewport;

	ID3D11InputLayout*              shadowInputLayout;
	ID3D11VertexShader*             shadowVS;

	ID3D11Buffer*					matrixBuffer;

public:

	ShadowMap(ID3D11Device* device, float dimensions, LPCWSTR vsFilename);
	virtual ~ShadowMap();

	ID3D11ShaderResourceView* GetShadowSRV();

	void ActivateShadowRendering(ID3D11DeviceContext* deviceContext);
	void SetBuffer(ID3D11DeviceContext* deviceContext, XMMATRIX& wvp);
};

