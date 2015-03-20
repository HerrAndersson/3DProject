#include "ShaderDefault.h"

using namespace DirectX;
using namespace std;

ShaderDefault::ShaderDefault(ID3D11Device* device,
	LPCWSTR vertexShaderFilename,
	LPCWSTR pixelShaderFilename
	) : ShaderBase(device)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
}

ShaderDefault::~ShaderDefault()
{
	matrixBuffer->Release();
}

void ShaderDefault::UseShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(hullShader, nullptr, 0);
	deviceContext->DSSetShader(domainShader, nullptr, 0);
	deviceContext->GSSetShader(geometryShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
}

void ShaderDefault::SetMatrices(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

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

	int bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);
	deviceContext->IASetInputLayout(inputLayout);
}

void* ShaderDefault::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void ShaderDefault::operator delete(void* p)
{
	_mm_free(p);
}

