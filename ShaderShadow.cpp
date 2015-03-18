#include "ShaderShadow.h"


ShaderShadow::ShaderShadow(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename, D3D11_INPUT_ELEMENT_DESC* inputDesc, unsigned int inputDescSize)
	:ShaderBase(device)
{
}

ShaderShadow::~ShaderShadow()
{
}
