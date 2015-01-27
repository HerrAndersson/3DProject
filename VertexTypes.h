#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct VertexPosCol
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};