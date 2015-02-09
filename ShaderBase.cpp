#include "ShaderBase.h"

using namespace std;

ShaderBase::ShaderBase(ID3D11Device* device)
{
	vertexShader = nullptr;
	hullShader = nullptr;
	pixelShader = nullptr;
	geometryShader = nullptr;
	domainShader = nullptr;
}

ShaderBase::~ShaderBase()
{
	if (vertexShader)
		vertexShader->Release();

	if (pixelShader)
		pixelShader->Release();

	if (hullShader)
		hullShader->Release();

	if (geometryShader)
		geometryShader->Release();

	if (domainShader)
		domainShader->Release();
}

void ShaderBase::CreateMandatoryShaders(ID3D11Device* device, string vertexShaderFilename, string pixelShaderFilename, D3D11_INPUT_ELEMENT_DESC* inputDesc, unsigned int inputDescSize)
{
	HRESULT hr;
	ID3DBlob* errorMessage = nullptr;

	//Create vertex shader
	LPCWSTR vsFilename = wstring(vertexShaderFilename.begin(), vertexShaderFilename.end()).c_str();
	ID3DBlob* pVS = nullptr;
	hr = D3DCompileFromFile(vsFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", NULL, NULL, &pVS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			throw runtime_error(string(static_cast<const char *>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			throw runtime_error("No such file: " + vertexShaderFilename);
		}
	}
	device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &vertexShader);

	//Create vertex layout
	device->CreateInputLayout(inputDesc, inputDescSize, pVS->GetBufferPointer(), pVS->GetBufferSize(), &inputLayout);
	pVS->Release();

	//Create pixel shader.
	LPCWSTR psFilename = wstring(pixelShaderFilename.begin(), pixelShaderFilename.end()).c_str();
	ID3DBlob* pPS = nullptr;
	hr = D3DCompileFromFile(psFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", NULL, NULL, &pPS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			throw runtime_error(string(static_cast<const char *>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			throw runtime_error("No such file: " + pixelShaderFilename);
		}
	}

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &pixelShader);
	pPS->Release();
}
