#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <DirectXMath.h>

#include "Object.h"
#include "BoundingBox.h"

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

private:
	void Clean(Node* currentNode);
	Node* ReadNode(ID3D11Device* device, std::ifstream& file);
};

