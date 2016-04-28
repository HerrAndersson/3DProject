#include "ShadowMap.h"


ShadowMap::ShadowMap(ID3D11Device* device, float dimensions, LPCWSTR vsFilename)
{
	this->dimensions = dimensions;

	HRESULT hr;
	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* pVS = nullptr;

	///////////////////////////////////////////////////////// T2D, DSV, SRV /////////////////////////////////////////////////////////
	D3D11_TEXTURE2D_DESC shadowMapDesc;
	ZeroMemory(&shadowMapDesc, sizeof(D3D11_TEXTURE2D_DESC));
	shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.ArraySize = 1;
	shadowMapDesc.SampleDesc.Count = 1;
	shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	shadowMapDesc.Height = (UINT)dimensions;
	shadowMapDesc.Width = (UINT)dimensions;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = device->CreateTexture2D(&shadowMapDesc, nullptr, &shadowMap);
	if (FAILED(hr))
	{
		throw runtime_error("Could not create Shadow map Texture2D");
	}
	hr = device->CreateDepthStencilView(shadowMap, &depthStencilViewDesc, &shadowDepthView);
	if (FAILED(hr))
	{
		throw runtime_error("Could not create Shadow map DSV");
	}
	hr = device->CreateShaderResourceView(shadowMap, &shaderResourceViewDesc, &shadowResourceView);
	if (FAILED(hr))
	{
		throw runtime_error("Could not create Shadow map SRV");
	}

	///////////////////////////////////////////////////////// Vertex shader /////////////////////////////////////////////////////////
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	hr = D3DCompileFromFile(vsFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", NULL, NULL, &pVS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			throw runtime_error(string(static_cast<const char *>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			throw runtime_error("Shadow Vertex file missing");
		}
	}

	device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &shadowVS);
	hr = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &shadowInputLayout);
	pVS->Release();

	///////////////////////////////////////////////////////////// Other /////////////////////////////////////////////////////////////

	//Create matrix buffer
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	if (FAILED(hr))
	{
		throw runtime_error("Could not create Shadow buffer");
	}

	//Init viewport
	ZeroMemory(&shadowViewport, sizeof(D3D11_VIEWPORT));
	shadowViewport.Height = dimensions;
	shadowViewport.Width = dimensions;
	shadowViewport.MinDepth = 0.f;
	shadowViewport.MaxDepth = 1.f;

	//Raster state with FRONT face culling
	D3D11_RASTERIZER_DESC shadowRasterDesc;
	ZeroMemory(&shadowRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	shadowRasterDesc.CullMode = D3D11_CULL_FRONT;
	shadowRasterDesc.FillMode = D3D11_FILL_SOLID;
	shadowRasterDesc.DepthClipEnable = true;

	hr = device->CreateRasterizerState(&shadowRasterDesc, &shadowRasterState);
	if (FAILED(hr))
	{
		throw runtime_error("Could not create Shadow raster state");
	}

	//Depth stencil state
	D3D11_DEPTH_STENCIL_DESC shadowDepthStencilDesc;
	ZeroMemory(&shadowDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	shadowDepthStencilDesc.DepthEnable = true;
	shadowDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	shadowDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	shadowDepthStencilDesc.StencilEnable = false;

	hr = device->CreateDepthStencilState(&shadowDepthStencilDesc, &shadowDepthState);
	if (FAILED(hr))
	{
		throw std::runtime_error("Shadow Depth stencil error");
	}
}

ShadowMap::~ShadowMap()
{
	shadowMap->Release();
	shadowDepthView->Release();
	shadowResourceView->Release();
}

void ShadowMap::ActivateShadowRendering(ID3D11DeviceContext* deviceContext)
{
	ID3D11ShaderResourceView* nullSrv = nullptr;
	deviceContext->PSSetShaderResources(0, 1, &nullSrv);

	deviceContext->ClearDepthStencilView(shadowDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	deviceContext->OMSetRenderTargets(0, nullptr, shadowDepthView);
	deviceContext->RSSetState(shadowRasterState);
	deviceContext->RSSetViewports(1, &shadowViewport);
	deviceContext->IASetInputLayout(shadowInputLayout);
	deviceContext->VSSetShader(shadowVS, nullptr, 0);
	deviceContext->OMSetDepthStencilState(shadowDepthState, 1);
	
	//Unbind PS since it's not used
	ID3D11PixelShader* nullPS = nullptr;
	deviceContext->PSSetShader(nullPS, nullptr, 0);
}

void ShadowMap::SetBuffer(ID3D11DeviceContext* deviceContext, XMMATRIX& wvp)
{
	//Set constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr;

	hr = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	MatrixBuffer* matrixDataBuffer = (MatrixBuffer*)mappedResource.pData;
	XMMATRIX twvp = XMMatrixTranspose(wvp);
	matrixDataBuffer->wvp = twvp;

	deviceContext->Unmap(matrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
}

ID3D11ShaderResourceView* ShadowMap::GetShadowSRV()
{
	return shadowResourceView;
}