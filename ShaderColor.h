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

	ID3D11Buffer* matrixBuffer;

public:

	ShaderColor(ID3D11Device* device, string vertexShaderFilename, string pixelShaderFilename, string hullShaderFilename = "", string geometryShaderFilename = "", string domainShaderFilename = "");
	virtual ~ShaderColor();

	virtual void UseShader(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projMatrix);


};

