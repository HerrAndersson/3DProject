#pragma once
#include <d3d11.h>

class ShadowMap
{

private:

	int width, height;

	ID3D11Texture2D*						shadowMap;
	ID3D11DepthStencilView*					shadowMapDepthView;
	ID3D11ShaderResourceView*				shadowMapSRV;
	D3D11_VIEWPORT							shadowMapViewport;
	float									shadowMapBias;

public:

	ShadowMap(ID3D11Device* device, int width, int height, float bias);
	virtual ~ShadowMap();

	ID3D11ShaderResourceView* GetShadowSRV();

	void ActivateShadowing(ID3D11DeviceContext* deviceContext);

	int GetSize();

	void ToggleShadowMapSize();
	void SetShadowMapBias(float bias);
};

