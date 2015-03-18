#include "BoundingBox.h"

using namespace std;
using namespace DirectX;

BoundingBox::BoundingBox()
{
	this->position = XMFLOAT2(0, 0);
	this->size = XMFLOAT2(0, 0);
}

BoundingBox::BoundingBox(XMFLOAT2 position, XMFLOAT2 size)
{
	this->position = position;
	this->size = size;
}

BoundingBox::~BoundingBox()
{
}

XMFLOAT2 BoundingBox::GetPosition()
{
	return position;
}

XMFLOAT2 BoundingBox::GetSize()
{
	return size;
}

BoundingBox BoundingBox::GetChildBoundingBox(int childQuadrant)
{
	XMFLOAT2 childPosition;
	XMFLOAT2 childSize = XMFLOAT2(size.x / 2, size.y / 2);

	switch (childQuadrant)
	{
	case 0:
		childPosition = XMFLOAT2(position.x + size.x / 2, position.y + size.y / 2);
		break;
	case 1:
		childPosition = XMFLOAT2(position.x, position.y + size.y / 2);
		break;
	case 2:
		childPosition = XMFLOAT2(position.x, position.y);
		break;
	case 3:
		childPosition = XMFLOAT2(position.x + size.x / 2, position.y);
		break;
	default:
		break;
	}

	return BoundingBox(childPosition, childSize);
}