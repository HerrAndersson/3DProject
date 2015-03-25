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
	for (vector<Object*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		delete *i;
	}
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

void Quadtree::Render(ID3D11DeviceContext* deviceContext, ShaderDefault* shader, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	int modelsRendered = 0;
	Render(deviceContext, root, rootBoundingBox, shader, viewMatrix, projectionMatrix, modelsRendered);
	cout << "Models rendered: " << modelsRendered << endl;
}

void Quadtree::Render(ID3D11DeviceContext* deviceContext, Node* currentNode, BoundingBox box, ShaderDefault* shader, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, int& modelsRendered)
{
	Plane planes[6];
	XMFLOAT4X4 m;
	XMMATRIX tempMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	XMStoreFloat4x4(&m, tempMatrix);

	//Left
	planes[0].normal.x =	-(m._14 + m._11);
	planes[0].normal.y =	-(m._24 + m._21);
	planes[0].normal.z =	-(m._34 + m._31);
	planes[0].d =			-(m._44 + m._41);

	//Right
	planes[1].normal.x =	-(m._14 - m._11);
	planes[1].normal.y =	-(m._24 - m._21);
	planes[1].normal.z =	-(m._34 - m._31);
	planes[1].d =			-(m._44 - m._41);

	//Top
	planes[2].normal.x =	-(m._14 - m._12);
	planes[2].normal.y =	-(m._24 - m._22);
	planes[2].normal.z =	-(m._34 - m._32);
	planes[2].d =			-(m._44 - m._42);

	//Bottom
	planes[3].normal.x =	-(m._14 + m._12);
	planes[3].normal.y =	-(m._24 + m._22);
	planes[3].normal.z =	-(m._34 + m._32);
	planes[3].d =			-(m._44 + m._42);

	//Near
	planes[4].normal.x =	-(m._14 + m._13);
	planes[4].normal.y =	-(m._24 + m._23);
	planes[4].normal.z =	-(m._34 + m._33);
	planes[4].d =			-(m._44 + m._43);

	//Far
	planes[5].normal.x =	-(m._14 - m._13);
	planes[5].normal.y =	-(m._24 - m._23);
	planes[5].normal.z =	-(m._34 - m._33);
	planes[5].d =			-(m._44 - m._43);

	//Normalize
	for (int i = 0; i < 6; i++)
	{
		XMVECTOR lengthVector = XMVector3Length(XMLoadFloat3(&planes[i].normal));
		XMFLOAT3 length;
		XMStoreFloat3(&length, lengthVector);
		float denom = 1.0f / length.x;

		planes[i].normal.x	*= denom;
		planes[i].normal.y	*= denom;
		planes[i].normal.z	*= denom;
		planes[i].d			*= denom;
	}

	//Calculate the 8 points of the bounding box
	XMFLOAT3 points[8];
	XMFLOAT2 pos = box.GetPosition();
	XMFLOAT2 size = box.GetSize();
	float height = 64.0f;
	points[0].x = pos.x;
	points[0].y = 0;
	points[0].z = pos.y;

	points[1].x = pos.x + size.x;
	points[1].y = 0;
	points[1].z = pos.y;

	points[2].x = pos.x;
	points[2].y = 0;
	points[2].z = pos.y + size.y;

	points[3].x = pos.x + size.x;
	points[3].y = 0;
	points[3].z = pos.y + size.y;

	points[4].x = pos.x;
	points[4].y = height;
	points[4].z = pos.y;

	points[5].x = pos.x + size.x;
	points[5].y = height;
	points[5].z = pos.y;

	points[6].x = pos.x;
	points[6].y = height;
	points[6].z = pos.y + size.y;

	points[7].x = pos.x + size.x;
	points[7].y = height;
	points[7].z = pos.y + size.y;

	int frustumIntersection = PlanesVsPoints(planes, points);
	if (frustumIntersection <= 0) //If box is inside or intersecting the frustum
	{
		for (vector<Object*>::iterator i = currentNode->objects.begin(); i != currentNode->objects.end(); ++i)
		{
			XMMATRIX world;
			(*i)->GetWorldMatrix(world);
			shader->SetMatrices(deviceContext, world, viewMatrix, projectionMatrix);
			(*i)->Render(deviceContext);
			modelsRendered++;
		}
		for (int i = 0; i < 4; i++)
		{
			if (currentNode->child[i])
			{
				Render(deviceContext, currentNode->child[i], box.GetChildBoundingBox(i), shader, viewMatrix, projectionMatrix, modelsRendered);
			}
		}
	}
}

int Quadtree::PlanesVsPoints(Plane planes[], DirectX::XMFLOAT3 points[])
{
	int insideFrustumCount = 0;

	for (int i = 0; i < 6; i++)
	{
		int insidePlaneCount = 0;
		int isAllInside = 1;

		for (int j = 0; j < 8; j++)
		{
			if (PlaneVsPoint(planes[i], points[j]) > 0)
			{
				isAllInside = 0;
				insidePlaneCount++;
			}
		}

		if (insidePlaneCount == 8)
		{
			//All points outside of frustum
			return 1;
		}
		insideFrustumCount += isAllInside;
	}

	if (insideFrustumCount == 6)
	{
		//All points are within the frustum
		return -1;
	}
	//The frustum is intersecting the bounding box
	return 0;
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
			XMFLOAT4X4 worldTransform;
			file >> objectFilename;
			file >> worldTransform.m[0][0] >> worldTransform.m[0][1] >> worldTransform.m[0][2] >> worldTransform.m[0][3]
				>> worldTransform.m[1][0] >> worldTransform.m[1][1] >> worldTransform.m[1][2] >> worldTransform.m[1][3]
				>> worldTransform.m[2][0] >> worldTransform.m[2][1] >> worldTransform.m[2][2] >> worldTransform.m[2][3]
				>> worldTransform.m[3][0] >> worldTransform.m[3][1] >> worldTransform.m[3][2] >> worldTransform.m[3][3];

			
			currentNode->objects.push_back(new Object(device, objectFilename, XMLoadFloat4x4(&worldTransform)));
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
