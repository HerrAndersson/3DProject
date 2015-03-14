#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "VertexTypes.h"
#include "Texture.h"

class ObjectBase
{
protected:

	ID3D11Buffer* vertexBuffer;
	Texture* texture;
	int vertexCount;
	DirectX::XMFLOAT4X4 worldMatrix;

public:

	ObjectBase(DirectX::XMFLOAT4X4 worldMatrix);
	ObjectBase();
	virtual ~ObjectBase();

	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;
	virtual void Update(ID3D11DeviceContext* deviceContext, float frameTime);

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11ShaderResourceView* GetTexture() const;
	void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix) const;
};

