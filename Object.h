#pragma once

#include <d3d11.h>
#include <string>
#include <vector>
#include <fstream>

#include "float3.h"

#include "ObjectBase.h"

using namespace std;

class Object : public ObjectBase
{
private:

	std::vector<float3> vertices;
	std::vector<float3> uvs;
	std::vector<float3> normals;
	std::vector<Vertex> faces;

public:

	Object(std::string filename, ID3D11Device* device);
	virtual ~Object();

};

