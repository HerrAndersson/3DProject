#include "ShaderShadowMap.h"
#include <string>
#include <fstream>
using namespace std;

ShaderShadowMap::ShaderShadowMap(ID3D11Device* device, LPCWSTR vertexShaderFilename, int width, int height, float bias)
{

	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_DEPTH_STENCIL_DESC depthDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;

	//Create rasterizer state
	device->CreateRasterizerState(&rasterDesc, &rasterizerState);

	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthDesc.StencilEnable = false;
	depthDesc.StencilReadMask = 0xFF;
	depthDesc.StencilWriteMask = 0xFF;
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	device->CreateDepthStencilState(&depthDesc, &depthStencilState);

	HRESULT hr;
	ID3DBlob* errorMessage = nullptr;

	//Create vertex shader
	ID3DBlob* pVS = nullptr;
	hr = D3DCompileFromFile(vertexShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", NULL, NULL, &pVS, &errorMessage);

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
	device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &vertexShader);

	//Create vertex layout

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &inputLayout);
	pVS->Release();

	D3D11_BUFFER_DESC matrixBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	this->width = width;
	this->height = height;
	this->shadowMapBias = bias;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	//texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
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
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(shadowMap, &dsvDesc, &shadowMapDepthView);

	if (FAILED(hr))
	{
		throw std::runtime_error("Shadow map 2");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(shadowMap, &srvDesc, &shadowMapSRV);

	if (FAILED(hr))
	{
		throw std::runtime_error("Shadow map 3");
	}

	//shadowMap->Release();

	shadowMapViewport.Width = (FLOAT)width;
	shadowMapViewport.Height = (FLOAT)height;
	shadowMapViewport.MinDepth = 0.0f;
	shadowMapViewport.MaxDepth = 1.0f;
	shadowMapViewport.TopLeftX = 0;
	shadowMapViewport.TopLeftY = 0;
}

ShaderShadowMap::~ShaderShadowMap()
{
	rasterizerState->Release();
	depthStencilState->Release();
	vertexShader->Release();
	matrixBuffer->Release();
	inputLayout->Release();
	shadowMap->Release();
	shadowMapDepthView->Release();
	shadowMapSRV->Release();
}

void ShaderShadowMap::UseShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(nullptr, nullptr, 0);

	deviceContext->RSSetViewports(1, &shadowMapViewport);
	deviceContext->ClearDepthStencilView(shadowMapDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	deviceContext->OMSetRenderTargets(0, nullptr, shadowMapDepthView);
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
	deviceContext->RSSetState(rasterizerState);
}
void ShaderShadowMap::SetBuffers(ID3D11DeviceContext* deviceContext, XMMATRIX& lightWVP, XMFLOAT3 lightPos)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	XMMATRIX lWVP = XMMatrixTranspose(lightWVP);

	hr = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	MatrixBuffer* matrixDataBuffer = (MatrixBuffer*)mappedResource.pData;

	//Copy the matrices into the constant buffer.
	matrixDataBuffer->lightWVP = lWVP;
	matrixDataBuffer->lightPos = lightPos;

	deviceContext->Unmap(matrixBuffer, 0);

	int bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
	deviceContext->IASetInputLayout(inputLayout);
}

ID3D11ShaderResourceView* ShaderShadowMap::GetShadowSRV()
{
	return shadowMapSRV;
}

int ShaderShadowMap::GetSize()
{
	//Square texture. height = width
	return height;
}

void* ShaderShadowMap::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void ShaderShadowMap::operator delete(void* p)
{
	_mm_free(p);
}
