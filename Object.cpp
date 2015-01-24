#include "Object.h"

Object::Object(std::string filename, ID3D11Device* device) : ObjectBase(device)
{
	std::ifstream file(filename);

	while (!file.eof())
	{
		std::string command;
		file >> command;
		if (command == "v")
		{
			XMFLOAT3 tempVertex;
			file >> tempVertex.x;
			file >> tempVertex.y;
			file >> tempVertex.z;
			vertices.push_back(tempVertex);
		}
		else if (command == "vt")
		{
			XMFLOAT2 tempUv;
			file >> tempUv.x;
			file >> tempUv.y;
			uvs.push_back(tempUv);
		}
		else if (command == "vn")
		{
			XMFLOAT3 tempNormal;
			file >> tempNormal.x;
			file >> tempNormal.y;
			file >> tempNormal.z;
			normals.push_back(tempNormal);
		}
		else if (command == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				int index;
				file >> index;
				Vertex tempVertex;

				tempVertex.pos = vertices[index - 1];

				//Check if we can load UV coordinates
				if (file.peek() == '/')
				{
					file.get();
					file >> index;
					tempVertex.uv = uvs[index - 1];

					//Check if we can load normals
					if (file.peek() == '/')
					{
						file.get();
						file >> index;
						XMFLOAT3 normal = normals[index - 1];
						//TODO: Vertex does not have normals
					}
				}
				
				faces.push_back(tempVertex);
			}
		}
		else if (command == "mtllib")
		{
			//TODO: Load texture
		}
	}

	file.close();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * faces.size();

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &faces[0];
	device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
}


Object::~Object()
{
}

