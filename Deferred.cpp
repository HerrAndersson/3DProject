#include "Deferred.h"

using namespace std;
using namespace DirectX;

Deferred::Deferred(ID3D11Device* device, int textureWidth, int textureHeight)
{
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;

	InitializeBuffers(device);
	InitializeShaderData(device);
}

Deferred::~Deferred()
{
	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}

	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}

	if (sampleStateWrap)
	{
		sampleStateWrap->Release();
		sampleStateWrap = nullptr;
	}

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		if (shaderResourceViewArray[i])
		{
			shaderResourceViewArray[i]->Release();
			shaderResourceViewArray[i] = nullptr;
		}

		if (renderTargetViewArray[i])
		{
			renderTargetViewArray[i]->Release();
			renderTargetViewArray[i] = nullptr;
		}

		if (renderTargetTextureArray[i])
		{
			renderTargetTextureArray[i]->Release();
			renderTargetTextureArray[i] = nullptr;
		}
	}
}

void Deferred::InitializeShaderData(ID3D11Device* device)
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;

	//Wrap texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &sampleStateWrap);
	if (FAILED(result))
	{
		throw runtime_error("Error creating sampler state");
	}

	//Description of the matrix constant buffer in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	if (FAILED(result))
	{
		throw runtime_error("Error creating matrix constant-buffer");
	}
}

void Deferred::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	//Render target texture description
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//Create the render target textures
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureArray[i]);
		if (FAILED(result))
		{
			throw runtime_error("Error creating render target textures");
		}
	}

	//Description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	//Create the render target views
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateRenderTargetView(renderTargetTextureArray[i], &renderTargetViewDesc, &renderTargetViewArray[i]);
		if (FAILED(result))
		{
			throw runtime_error("Error creating render target views");
		}
	}

	//Description of the shader resource view
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	//Create the shader resource views
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		result = device->CreateShaderResourceView(renderTargetTextureArray[i], &shaderResourceViewDesc, &shaderResourceViewArray[i]);
		if (FAILED(result))
		{
			throw runtime_error("Error creating shader resource views");
		}
	}

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	
	//Description of the depth buffer
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if (FAILED(result))
	{
		throw runtime_error("Error creating texture for the depth buffer");
	}

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//Stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create the depth stencil view
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result))
	{
		throw runtime_error("Error creating depth stencil view");
	}

	//Viewport for rendering
	viewport.Width = (float)textureWidth;
	viewport.Height = (float)textureHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

}

void Deferred::SetRenderTargets(ID3D11DeviceContext* deviceContext)
{
	//Bind the render target view array and depth stencil buffer to the output render pipeline
	deviceContext->OMSetRenderTargets(BUFFER_COUNT, renderTargetViewArray, depthStencilView);

	deviceContext->RSSetViewports(1, &viewport);
}
void Deferred::ClearRenderTargets(ID3D11DeviceContext* deviceContext, float r, float g, float b, float a)
{
	float color[4] = { r, g, b, a };

	//Clear the render target buffers
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		deviceContext->ClearRenderTargetView(renderTargetViewArray[i], color);
	}

	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

ID3D11ShaderResourceView* Deferred::GetShaderResourceView(int viewNumber)
{
	return shaderResourceViewArray[viewNumber];
}

//void Deferred::SetBuffers(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture)
//{
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//
//	XMMATRIX wm = XMMatrixTranspose(worldMatrix);
//	XMMATRIX vm = XMMatrixTranspose(viewMatrix);
//	XMMATRIX pm = XMMatrixTranspose(projectionMatrix);
//
//	//Lock the constant buffer so it can be written to
//	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result))
//	{
//		throw runtime_error("Could not Map matrix buffer in Deferred.cpp");
//	}
//
//	MatrixBuffer* matrixData = (MatrixBuffer*)mappedResource.pData;
//
//	//Copy the matrices into the constant buffer
//	matrixData->world = wm;
//	matrixData->view = vm;
//	matrixData->projection = pm;
//
//	deviceContext->Unmap(matrixBuffer, 0);
//
//	unsigned int bufferNumber = 0;
//	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
//	deviceContext->PSSetShaderResources(0, 1, &texture);
//}
//
//void Deferred::UseShader(ID3D11DeviceContext* deviceContext)
//{
//	deviceContext->VSSetShader(vertexShader, nullptr, 0);
//	deviceContext->HSSetShader(hullShader, nullptr, 0);
//	deviceContext->DSSetShader(domainShader, nullptr, 0);
//	deviceContext->GSSetShader(geometryShader, nullptr, 0);
//	deviceContext->PSSetShader(pixelShader, nullptr, 0);
//	deviceContext->IASetInputLayout(inputLayout);
//	deviceContext->PSSetSamplers(0, 1, &sampleStateWrap);
//}

void* Deferred::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void Deferred::operator delete(void* p)
{
	_mm_free(p);
}
