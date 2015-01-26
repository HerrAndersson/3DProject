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
	XMVECTOR position;
	XMVECTOR color;
};

