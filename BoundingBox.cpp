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
