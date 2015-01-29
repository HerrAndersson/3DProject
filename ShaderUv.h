#pragma once
#include "ShaderBase.h"
class ShaderUv : public ShaderBase
{

private:

	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	ID3D11Buffer*				matrixBuffer;

public:

	ShaderUv();
	virtual ~ShaderUv();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	virtual void UseShader(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projMatrix);


};

