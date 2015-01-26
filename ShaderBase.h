#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <d3dcompiler.h>
#include <string>
#include <fstream>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace std;

class ShaderBase
{
private:

	ID3D11VertexShader*    vertexShader;
	ID3D11InputLayout*     vertexLayout;
	ID3D11PixelShader*     pixelShader;

	void OutputErrorMessage(ID3DBlob* errorMessage, HWND hwnd);

public:

	ShaderBase();
	virtual ~ShaderBase();

	bool Initialize(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename, HWND hwnd);
	void UseShader(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer);

	//bool Update();
};

