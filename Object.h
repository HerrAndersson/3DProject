#pragma once

#include <d3d11.h>
#include <string>
#include <vector>
#include <fstream>

#include "float3.h"
#include "Vertex.h"

class Object
{
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11ShaderResourceView* texture;
	std::vector<float3> vertices;
	std::vector<float3> uvs;
	std::vector<float3> normals;
	std::vector<Vertex> faces;
public:
	Object(std::string filename, ID3D11Device* device);
	~Object();

	ID3D11Buffer* getVertexBuffer() const;
	ID3D11ShaderResourceView* getTextureView() const;
};

