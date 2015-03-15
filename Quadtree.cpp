#include "Quadtree.h"

using namespace std;
using namespace DirectX;

Quadtree::Node::Node()
{
	for (int i = 0; i < 4; i++)
	{
		child[i] = nullptr;
	}
}

Quadtree::Node::~Node()
{
}

Quadtree::Quadtree(ID3D11Device* device, std::string filename)
{
	ifstream file(filename);
	if (file.good())
	{
		XMFLOAT2 position;
		XMFLOAT2 size;
		file >> position.x >> position.y >> size.x >> size.y;
		rootBoundingBox = BoundingBox(position, size);
		root = ReadNode(device, file);
	}
	file.close();
}

Quadtree::~Quadtree()
{
	Clean(root);
}

void Quadtree::Render(ID3D11DeviceContext* deviceContext, const XMFLOAT3& campos, const XMFLOAT3& camdir)
{
	Render(deviceContext, campos, camdir, root);
}

void Quadtree::Render(ID3D11DeviceContext* deviceContext, const XMFLOAT3& campos, const XMFLOAT3& camdir, Node* currentNode)
{
	for (vector<Object*>::iterator i = currentNode->objects.begin(); i != currentNode->objects.end(); ++i)
	{
		(*i)->Render(deviceContext);
	}
	for (int i = 0; i < 4; i++)
	{
		if (currentNode->child[i])
		{
			Render(deviceContext, campos, camdir, currentNode->child[i]);
		}
	}
}

void Quadtree::Clean(Node* currentNode)
{
	if (currentNode)
	{
		for (int i = 0; i < 4; i++)
		{
			Clean(currentNode->child[i]);
		}
		delete currentNode;
		currentNode = nullptr;
	}
}

Quadtree::Node* Quadtree::ReadNode(ID3D11Device* device, std::ifstream& file)
{
	string command;
	
	Node* currentNode = new Node();
	while (!file.eof())
	{
		file >> command;
		if (command == "o")
		{
			string objectFilename;
			string textureFilename;
			XMFLOAT4X4 worldTransform;
			file >> objectFilename >> textureFilename;
			file >> worldTransform.m[0][0] >> worldTransform.m[0][1] >> worldTransform.m[0][2] >> worldTransform.m[0][3]
				>> worldTransform.m[1][0] >> worldTransform.m[1][1] >> worldTransform.m[1][2] >> worldTransform.m[1][3]
				>> worldTransform.m[2][0] >> worldTransform.m[2][1] >> worldTransform.m[2][2] >> worldTransform.m[2][3]
				>> worldTransform.m[3][0] >> worldTransform.m[3][1] >> worldTransform.m[3][2] >> worldTransform.m[3][3];

			
			currentNode->objects.push_back(new Object(device, objectFilename, textureFilename, XMLoadFloat4x4(&worldTransform)));
		}
		else if (command == "c")
		{
			int childNum;
			file >> childNum;
			currentNode->child[childNum] = ReadNode(device, file);
		}
		else if (command == "e")
		{
			return currentNode;
		}
	}
}
