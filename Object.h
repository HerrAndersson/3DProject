#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <fstream>

#include "ObjectBase.h"

using namespace std;
using namespace DirectX;

class Object : public ObjectBase
{
private:
	vector<XMFLOAT3> vertices;
	vector<XMFLOAT2> uvs;
	vector<XMFLOAT3> normals;
	vector<Vertex> faces;

public:

	Object(std::string modelFilename, string textureFilename, ID3D11Device* device);
	virtual ~Object();

};

