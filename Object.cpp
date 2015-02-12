#include "Object.h"
#include "Texture.h"

using namespace std;
using namespace DirectX;

Object::Object(string filename, string textureFilename, ID3D11Device* device) : ObjectBase()
{
	bool result = true;
	ifstream file(filename);

	if (file.good())
	{
		while (!file.eof())
		{
			string command;
			file >> command;
			if (command == "v")
			{
				XMFLOAT3 tempVertex;
				file >> tempVertex.x;
				file >> tempVertex.y;
				file >> tempVertex.z;
				tempVertex.z *= -1.0f;
				vertices.push_back(tempVertex);
			}
			else if (command == "vt")
			{
				XMFLOAT2 tempUv;
				file >> tempUv.x;
				file >> tempUv.y;
				tempUv.y = 1.0f - tempUv.y;
				uvs.push_back(tempUv);
			}
			else if (command == "vn")
			{
				XMFLOAT3 tempNormal;
				file >> tempNormal.x;
				file >> tempNormal.y;
				file >> tempNormal.z;
				tempNormal.z *= -1.0f;
				normals.push_back(tempNormal);
			}
			else if (command == "f")
			{
				int vertexIndex[3];
				int uvIndex[3];
				int normalIndex[3];

				for (int i = 0; i < 3; i++)
				{
					file >> vertexIndex[i];
					//Check if we can load UV coordinates
					if (file.peek() == '/')
					{
						file.get();
						file >> uvIndex[i];
						//Check if we can load normals
						if (file.peek() == '/')
						{
							file.get();
							file >> normalIndex[i];
						}
					}
				}
				for (int i = 2; i >= 0; i--)
				{
					VertexPosUV tempVertex;
					tempVertex.pos = vertices[vertexIndex[i] - 1];
					//tempVertex.uv = uvs[uvIndex[i] - 1];
					//tempVertex.norm = normals[normalIndex[i] - 1];
					//TODO: Vertex does not have normals
					faces.push_back(tempVertex);
				}
				
				

				
			}
			else if (command == "mtllib")
			{
				//TODO: Assumes object has a texture
				Texture* tempTexture = new Texture(textureFilename, device);
				texture = tempTexture;
			}
		}
	}
	else
	{
		result = false;
	}
	file.close();

	if (result)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VertexPosUV) * faces.size();

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &faces[0];
		device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	}
	else
	{
		MessageBoxA(nullptr, "Failed to load file", string("No such file: " + filename).c_str(), MB_OK);
	}
}


Object::~Object()
{
}

