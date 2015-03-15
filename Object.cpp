#include "Object.h"
#include "Texture.h"

using namespace std;
using namespace DirectX;

Object::Object(ID3D11Device* device, std::string modelFilename, std::string textureFilename, DirectX::XMMATRIX& worldMatrix) : ObjectBase(worldMatrix)
{
	bool result = true;
	ifstream file(modelFilename);

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
					Vertex tempVertex;
					tempVertex.pos = vertices[vertexIndex[i] - 1];
					tempVertex.uv = uvs[uvIndex[i] - 1];
					tempVertex.normal = normals[normalIndex[i] - 1];
					faces.push_back(tempVertex);
				}
			}
			else if (command == "mtllib")
			{
				//TODO: Assumes object has a texture
				texture = new Texture(textureFilename, device);
			}
		}
	}
	else
	{
		throw runtime_error("Failed to load file: " + modelFilename);
	}
	file.close();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * faces.size();

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = faces.data();
	device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	vertexCount = faces.size();
}

Object::~Object()
{
}

void Object::Render(ID3D11DeviceContext* deviceContext)
{
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	ID3D11ShaderResourceView* textureView = GetTexture();

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->PSSetShaderResources(0, 1, &textureView);

	deviceContext->Draw(vertexCount, 0);
}


