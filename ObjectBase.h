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

public:

	ObjectBase();
	virtual ~ObjectBase();

	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11ShaderResourceView* GetTexture() const;
};

