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

	ShaderBase(ID3D11Device* device, HWND hwnd, D3D11_INPUT_ELEMENT_DESC* inputDesc, UINT idSize, WCHAR* vsFilename, WCHAR* psFilename);
	virtual ~ShaderBase();

	//If vertexBuffer = nullptr, then the vertex buffer is set from elsewhere, ex. in the object itself
	virtual void UseShader(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer = nullptr);

	//bool Update();
};

