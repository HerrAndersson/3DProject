#pragma once
#include <DirectXMath.h>

struct VertexPosUV
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

struct VertexPosCol
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
};

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};