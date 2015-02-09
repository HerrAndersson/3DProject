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
	std::vector<VertexPosUV> faces;

public:

	Object(std::string modelFilename, std::string textureFilename, ID3D11Device* device);
	virtual ~Object();

};

