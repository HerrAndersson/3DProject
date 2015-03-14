#pragma once
#include <DirectXMath.h>

class BoundingBox
{
private:
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 size;
public:
	BoundingBox();
	BoundingBox(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
	~BoundingBox();

	DirectX::XMFLOAT2 GetPosition();
	DirectX::XMFLOAT2 GetSize();
};
