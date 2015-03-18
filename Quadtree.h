#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <DirectXMath.h>

#include "Object.h"
#include "BoundingBox.h"

#include "ShaderDefault.h"
#include "Intersections.h"

class Quadtree
{
private:
	class Node
	{
	public:
		Node();
		~Node();
		Node* child[4];
		std::vector<Object*> objects;
	};
	Node* root;
	BoundingBox rootBoundingBox;
	const int MAX_DEPTH = 4;
public:
	Quadtree(BoundingBox rootBoundingBox);
	Quadtree(ID3D11Device* device, std::string filename);
	~Quadtree();

	void Render(ID3D11DeviceContext* deviceContext, ShaderDefault* shader, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
	void Render(ID3D11DeviceContext* deviceContext, Node* currentNode, BoundingBox box, ShaderDefault* shader, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);

private:
	void Clean(Node* currentNode);
	Node* ReadNode(ID3D11Device* device, std::ifstream& file);
	int PlanesVsPoints(Plane planes[], DirectX::XMFLOAT3 points[]);
};

