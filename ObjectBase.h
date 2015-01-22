#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"

class ObjectBase
{
protected:

	ID3D11Buffer* vertexBuffer;
	ID3D11ShaderResourceView* texture;

private:

	int vertexCount;

public:

	ObjectBase(ID3D11Device* device);
	~ObjectBase();

	void Render(ID3D11DeviceContext* deviceContext);

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11ShaderResourceView* GetTextureView() const;
};

