#include "ShaderLight.h"

using namespace std;
using namespace DirectX;

ShaderLight::ShaderLight(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename)
	: ShaderBase(device)
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateMandatoryShaders(device, vertexShaderFilename, pixelShaderFilename, inputDesc, ARRAYSIZE(inputDesc));

	//Texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the texture sampler state
	result = device->CreateSamplerState(&samplerDesc, &sampleState);
	if (FAILED(result))
	{
		throw runtime_error("Error creating sampler state");
	}


	// Description of the light constant buffer in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferPS);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	//Create light buffer pointer so the values withing the shader can be changed
	result = device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);
	if (FAILED(result))
	{
		throw runtime_error("Error creating ligth buffer");
	}
}


ShaderLight::~ShaderLight()
{
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = nullptr;
	}


	if (sampleState)
	{
		sampleState->Release();
		sampleState = nullptr;
	}
}

void ShaderLight::SetBuffers(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* colorTexture,
	ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* shadowTexture, ID3D11ShaderResourceView* worldPosTexture, DirectX::XMFLOAT3 lightDirection, XMMATRIX& lightVP, int shadowMapSize)

{

	//Update light constant buffer
	SetLightBuffer(deviceContext, lightDirection, shadowMapSize, lightVP);

	//Set shader texture resources in the pixel shader
	deviceContext->PSSetShaderResources(0, 1, &colorTexture);
	deviceContext->PSSetShaderResources(1, 1, &normalTexture);
	deviceContext->PSSetShaderResources(2, 1, &shadowTexture);
	deviceContext->PSSetShaderResources(3, 1, &worldPosTexture);
}

void ShaderLight::Draw(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(hullShader, nullptr, 0);
	deviceContext->DSSetShader(domainShader, nullptr, 0);
	deviceContext->GSSetShader(geometryShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->PSSetSamplers(0, 1, &sampleState);

	//Unlike the other shaders who lets the objects draw themselves, after activating the shader and setting all values and data, this shader has to render the whole scene
	//since the objects have already drawn themselves to the buffer used in ShaderDeferred
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void ShaderLight::SetLightBuffer(ID3D11DeviceContext* deviceContext, XMFLOAT3 lightDirection, int shadowMapSize, XMMATRIX& lightVP)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber = 0;

	XMMATRIX lvp = XMMatrixTranspose(lightVP);

	//Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		throw runtime_error("Could not Map light buffer in ShaderLight");
	}

	LightBufferPS* lightData = (LightBufferPS*)mappedResource.pData;

	//Copy the lighting variables into the constant buffer
	lightData->lightVP = lvp;
	lightData->lightDirection = lightDirection;
	lightData->size = shadowMapSize;

	deviceContext->Unmap(lightBuffer, 0);

	//Set light buffer in pixel shader with updated values
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &lightBuffer);
}
