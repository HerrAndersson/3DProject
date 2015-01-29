#include "ShaderColor.h"

using namespace DirectX;

ShaderColor::ShaderColor() : ShaderBase()
{
}

ShaderColor::~ShaderColor()
{
}

bool ShaderColor::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
	{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (!((ShaderBase*)this)->Initialize(device, hwnd, inputDesc, ARRAYSIZE(inputDesc), vsFilename, psFilename))
	{
		return false;
	}

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
		return false;
	}

	return true;
}

void ShaderColor::UseShader(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projMatrix)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* matrixDataBuffer;
	unsigned int bufferNumber;

	XMMATRIX wm = XMMatrixTranspose(worldMatrix);
	XMMATRIX vm = XMMatrixTranspose(viewMatrix);
	XMMATRIX pm = XMMatrixTranspose(projMatrix);

	hr = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	matrixDataBuffer = (MatrixBuffer*)mappedResource.pData;

	//Copy the matrices into the constant buffer.
	matrixDataBuffer->world = wm;
	matrixDataBuffer->view = vm;
	matrixDataBuffer->projection = pm;

	deviceContext->Unmap(matrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
}
