#include "ShaderShadowMap.h"
#include <string>
#include <fstream>
using namespace std;

ShaderShadowMap::ShaderShadowMap(ID3D11Device* device, LPCWSTR vertexShaderFilename)
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
	depthDesc.StencilEnable = true;
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
}

ShaderShadowMap::~ShaderShadowMap()
{
	rasterizerState->Release();
	depthStencilState->Release();
	vertexShader->Release();
	matrixBuffer->Release();
	inputLayout->Release();
}

void ShaderShadowMap::UseShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->RSSetState(rasterizerState);
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
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

void* ShaderShadowMap::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void ShaderShadowMap::operator delete(void* p)
{
	_mm_free(p);
}
