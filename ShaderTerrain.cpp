#include "ShaderTerrain.h"
#include <iostream>

using namespace DirectX;
using namespace std;

ShaderTerrain::ShaderTerrain(ID3D11Device* device, LPCWSTR vsFilename, LPCWSTR psFilename, LPCWSTR gsFilename)
			 : ShaderBase(device)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateMandatoryShaders(device, vsFilename, psFilename, inputDesc, ARRAYSIZE(inputDesc));

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

	D3D11_BUFFER_DESC gsBufferDesc;

	// Setup the description of the dynamic constant buffer that is in the geometry shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	gsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	gsBufferDesc.ByteWidth = sizeof(GSBuffer);
	gsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gsBufferDesc.MiscFlags = 0;
	gsBufferDesc.StructureByteStride = 0;

	//Create the gsBuffer pointer so we can access the geometry shader constant buffer from within this class.
	hr = device->CreateBuffer(&gsBufferDesc, NULL, &gsBuffer);

	if (FAILED(hr))
	{
		throw runtime_error("Could not create gsBuffer");
	}

	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
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

	//Create geometry shader.
	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* pPS = nullptr;
	hr = D3DCompileFromFile(gsFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_4_0", NULL, NULL, &pPS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			throw runtime_error(string(static_cast<const char *>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			throw runtime_error("No such file");
		}
	}
	device->CreateGeometryShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &geometryShader);
	pPS->Release();
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

void ShaderTerrain::SetBuffers(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, ID3D11ShaderResourceView** textures, XMFLOAT3 camPos)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	int bufferNumber;

	XMMATRIX wvp = worldMatrix * viewMatrix * projectionMatrix;
	wvp = XMMatrixTranspose(wvp);
	XMMATRIX wm = XMMatrixTranspose(worldMatrix);
	XMMATRIX vm = XMMatrixTranspose(viewMatrix);
	XMMATRIX pm = XMMatrixTranspose(projectionMatrix);

	///////////////////////////////////////////////// Matrix buffer, PS /////////////////////////////////////////////////
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

	/////////////////////////////////////////////////// GSBuffer, GS ///////////////////////////////////////////////////

	hr = deviceContext->Map(gsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	GSBuffer* gsDataBuffer = (GSBuffer*)mappedResource.pData;

	//Copy the data into the constant buffer.
	gsDataBuffer->camPos = camPos;
	gsDataBuffer->viewMatrix = viewMatrix;

	deviceContext->Unmap(gsBuffer, 0);

	bufferNumber = 0;

	//Set matrix buffer to the vertex shader
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &gsBuffer);

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

