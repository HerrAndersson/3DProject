#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "VertexTypes.h"
#include "Texture.h"

using namespace DirectX;

class ObjectBase
{
protected:

	ID3D11Buffer* vertexBuffer;
	Texture* texture;
	int vertexCount;
	XMMATRIX worldMatrix;

public:

	ObjectBase(DirectX::XMFLOAT4X4 worldMatrix);
	ObjectBase();
	virtual ~ObjectBase();

	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;
	virtual void Update(ID3D11DeviceContext* deviceContext, float frameTime);

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11ShaderResourceView* GetTexture() const;
	void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix) const;

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);
};

