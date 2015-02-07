#include "ShaderColor.h"

using namespace DirectX;

ShaderColor::ShaderColor(ID3D11Device* device, HWND hwnd, D3D11_INPUT_ELEMENT_DESC* inputDesc, UINT idSize, WCHAR* vsFilename, WCHAR* psFilename) 
	: ShaderBase(device, hwnd, inputDesc, idSize, vsFilename, psFilename)
{
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

ShaderColor::~ShaderColor()
{
}

void ShaderColor::UseShader(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projMatrix)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	XMMATRIX wm = XMMatrixTranspose(worldMatrix);
	XMMATRIX vm = XMMatrixTranspose(viewMatrix);
	XMMATRIX pm = XMMatrixTranspose(projMatrix);
	XMMATRIX wvp = XMMatrixTranspose(worldMatrix * (viewMatrix * projMatrix));

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
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(vertexLayout);
}
