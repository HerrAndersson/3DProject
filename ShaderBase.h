#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <d3dcompiler.h>
#include <string>
#include <fstream>

class ShaderBase
{

protected:

	ID3D11VertexShader*    vertexShader;
	ID3D11InputLayout*     vertexLayout;
	ID3D11PixelShader*     pixelShader;

private:

	virtual void OutputErrorMessage(ID3DBlob* errorMessage, HWND hwnd);

public:

	ShaderBase();
	virtual ~ShaderBase();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, D3D11_INPUT_ELEMENT_DESC* inputDesc, UINT idSize, WCHAR* vsFilename, WCHAR* psFilename);
	virtual void UseShader(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer);

	//bool Update();
};

