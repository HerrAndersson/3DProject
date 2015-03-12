#include "ShaderTerrain.h"
#include <iostream>

using namespace DirectX;
using namespace std;

ShaderTerrain::ShaderTerrain(	ID3D11Device* device,
							LPCWSTR vertexShaderFilename,
							LPCWSTR pixelShaderFilename
						):	ShaderBase(device)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateMandatoryShaders(device, vertexShaderFilename, pixelShaderFilename, inputDesc, ARRAYSIZE(inputDesc));

	D3D11_BUFFER_DESC matrixBufferDesc;
	HRESULT hr;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	if (FAILED(hr))
	{
		throw runtime_error("Could not create MatrixBuffer");
	}

	D3D11_BUFFER_DESC lightBufferDesc;

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	hr = device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	if (FAILED(hr))
	{
		throw runtime_error("Could not create LightBuffer");
	}

	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
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

	// Create the texture sampler state.
	hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(hr))
	{
		throw runtime_error("Could not create SamplerState");
	}
}

ShaderTerrain::~ShaderTerrain()
{
}

void ShaderTerrain::UseShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(hullShader, nullptr, 0);
	deviceContext->DSSetShader(domainShader, nullptr, 0);
	deviceContext->GSSetShader(geometryShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->IASetInputLayout(inputLayout);
}

void ShaderTerrain::SetBuffers(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, ID3D11ShaderResourceView** textures)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	int bufferNumber;

	XMMATRIX wvp = worldMatrix * viewMatrix * projectionMatrix;
	wvp = XMMatrixTranspose(wvp);
	XMMATRIX wm = XMMatrixTranspose(worldMatrix);
	XMMATRIX vm = XMMatrixTranspose(viewMatrix);
	XMMATRIX pm = XMMatrixTranspose(projectionMatrix);

	hr = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	MatrixBuffer* matrixDataBuffer = (MatrixBuffer*)mappedResource.pData;

	//Copy the matrices into the constant buffer.
	matrixDataBuffer->world = wm;
	matrixDataBuffer->view = vm;
	matrixDataBuffer->projection = pm;
	matrixDataBuffer->wvp = wvp;

	deviceContext->Unmap(matrixBuffer, 0);

	bufferNumber = 0;

	//Set matrix buffer to the vertex shader
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	//D3D11_MAPPED_SUBRESOURCE mr;
	//hr = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	//LightBuffer* lightDataBuffer = (LightBuffer*)mr.pData;

	////Copy the lighting variables into the constant buffer
	//lightDataBuffer->ambientColor = light->GetAmbientColor();
	//lightDataBuffer->diffuseColor = light->GetDiffuseColor();
	//lightDataBuffer->lightDirection = light->GetDirection();
	//lightDataBuffer->padding = padding;

	//deviceContext->Unmap(lightBuffer, 0);

	//bufferNumber = 0;

	////Set the light buffer in the pixel shader
	//deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);

	deviceContext->PSSetSamplers(0, 1, &samplerState);
	deviceContext->PSSetShaderResources(0, 4, textures);
}

void* ShaderTerrain::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void ShaderTerrain::operator delete(void* p)
{
	_mm_free(p);
}

