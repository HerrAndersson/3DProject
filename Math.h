#ifndef MATH_H
#define MATH_H
#include <DirectXMath.h>

using namespace DirectX;

static float DotProduct3(XMFLOAT3 v1, XMFLOAT3 v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

static XMFLOAT3 CrossProduct3(XMFLOAT3 v1, XMFLOAT3 v2)
{
	return XMFLOAT3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

static XMFLOAT3 Subtract3(XMFLOAT3 v1, XMFLOAT3 v2)
{
	return XMFLOAT3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

#endif