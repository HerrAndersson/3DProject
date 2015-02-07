#pragma once
#include "ShaderBase.h"

using namespace DirectX;

class ShaderColor : public ShaderBase
{

private:

	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX wvp;
	};

	ID3D11Buffer*				matrixBuffer;

public:

	ShaderColor(ID3D11Device* device, HWND hwnd, D3D11_INPUT_ELEMENT_DESC* inputDesc, UINT idSize, WCHAR* vsFilename, WCHAR* psFilename);
	virtual ~ShaderColor();

	virtual void UseShader(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projMatrix);


};

