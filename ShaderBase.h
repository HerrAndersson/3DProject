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
	ID3D11VertexShader*		vertexShader;
	ID3D11PixelShader*		pixelShader;
	ID3D11HullShader*		hullShader;
	ID3D11GeometryShader*	geometryShader;
	ID3D11DomainShader*		domainShader;

	ID3D11InputLayout*		inputLayout;

	unsigned int vertexSize;

public:

	ShaderBase(ID3D11Device* device);
	virtual ~ShaderBase();

	void CreateMandatoryShaders(ID3D11Device* device, string vertexShaderFilename, string pixelShaderFilename, D3D11_INPUT_ELEMENT_DESC* inputDesc, unsigned int inputDescSize);
	virtual void UseShader(ID3D11DeviceContext* deviceContext) = 0;
};

