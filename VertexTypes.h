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
	DirectX::XMFLOAT4 color;
};

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};

struct VertexParticles
{
	DirectX::XMFLOAT3 pos;
};