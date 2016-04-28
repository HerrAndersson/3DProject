#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <fstream>

#include "ObjectBase.h"

class Object : public ObjectBase
{
private:

	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<DirectX::XMFLOAT2> uvs;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<Vertex> faces;

public:

	Object(ID3D11Device* device, std::string modelFilename, DirectX::XMMATRIX& worldMatrix);
	virtual ~Object();

	virtual void Render(ID3D11DeviceContext* deviceContext);
};

