#include "ShaderBase.h"

ShaderBase::ShaderBase()
{
	vertexShader = nullptr;
	vertexLayout = nullptr;
	pixelShader = nullptr;
}

ShaderBase::~ShaderBase()
{
	vertexShader->Release();
	vertexLayout->Release();
	pixelShader->Release();

	vertexShader = nullptr;
	vertexLayout = nullptr;
	pixelShader = nullptr;
}

bool ShaderBase::Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename, HWND hwnd)
{
	//Create vertex shader.
	HRESULT hr;
	ID3DBlob* errorMessage = nullptr;

	ID3DBlob* pVS = nullptr;
	hr = D3DCompileFromFile(vsFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", NULL, NULL, &pVS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
			OutputErrorMessage(errorMessage, hwnd);
		else
			MessageBox(hwnd, vsFilename, L"Missing shader file", MB_OK);

		return false;
	}

	device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &vertexShader);

	//Create input layout (verified using vertex shader). 
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
	{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);
	pVS->Release();

	//Create pixel shader.
	ID3DBlob* pPS = nullptr;
	hr = D3DCompileFromFile(psFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", NULL, NULL, &pPS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
			OutputErrorMessage(errorMessage, hwnd);
		else
			MessageBox(hwnd, psFilename, L"Missing shader file", MB_OK);

		return false;
	}

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &pixelShader);
	pPS->Release();

	return true;
}

void ShaderBase::UseShader(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer)
{
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(nullptr, nullptr, 0);
	deviceContext->DSSetShader(nullptr, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(float) * 6;
	UINT32 offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(vertexLayout);
	 
}

void ShaderBase::OutputErrorMessage(ID3DBlob* errorMessage, HWND hwnd)
{
	ofstream fout;

	char* compileErrors = (char*)(errorMessage->GetBufferPointer());
	unsigned long bufferSize = errorMessage->GetBufferSize();

	//Open file and write error message to it
	fout.open("shader-error.txt");
	for (unsigned long i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}
	fout.close();

	errorMessage->Release();
	errorMessage = nullptr;

	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", L"Error", MB_OK);
}

