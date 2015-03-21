#include "ShadowMap.h"
#include <string>
using namespace std;

ShadowMap::ShadowMap(ID3D11Device* device, int width, int height, float bias)
{
	this->width = width;
	this->height = height;
	this->shadowMapBias = bias;
	HRESULT hr;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	//texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	
	hr = device->CreateTexture2D(&texDesc, nullptr, &shadowMap);

	if (FAILED(hr))
	{
		throw std::runtime_error("Shadow map 1");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Flags = 0;
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(shadowMap, &dsvDesc, &shadowMapDepthView);

	if (FAILED(hr))
	{
		throw std::runtime_error("Shadow map 2");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	//srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(shadowMap, &srvDesc, &shadowMapSRV);

	if (FAILED(hr))
	{
		throw std::runtime_error("Shadow map 3");
	}

	shadowMap->Release();

	shadowMapViewport.Width = (FLOAT)width;
	shadowMapViewport.Height = (FLOAT)height;
	shadowMapViewport.MinDepth = 0.0f;
	shadowMapViewport.MaxDepth = 1.0f;
	shadowMapViewport.TopLeftX = 0;
	shadowMapViewport.TopLeftY = 0;
}

ShadowMap::~ShadowMap()
{
	shadowMap->Release();
	shadowMapDepthView->Release();
	shadowMapSRV->Release();
}

void ShadowMap::SetShadowMapBias(float bias)
{
	shadowMapBias = bias;
}

ID3D11ShaderResourceView* ShadowMap::GetShadowSRV()
{
	return shadowMapSRV;
}

void ShadowMap::ActivateShadowing(ID3D11DeviceContext* deviceContext)
{
	deviceContext->RSSetViewports(1, &shadowMapViewport);

	deviceContext->OMSetRenderTargets(0, nullptr, shadowMapDepthView);

	//deviceContext->OMSetRenderTargets(0, 0, shadowMapDepthView);
	deviceContext->ClearDepthStencilView(shadowMapDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

int ShadowMap::GetSize()
{
	//Square texture. height = width
	return height;
}
