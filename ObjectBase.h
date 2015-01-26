#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "VertexTypes.h"
#include <string>

using namespace std;

class ObjectBase
{
protected:

	ID3D11Buffer* vertexBuffer;
	ID3D11ShaderResourceView* texture;

private:

	int vertexCount;

public:

	ObjectBase(ID3D11Device* device, string textureFilename);
	~ObjectBase();

	void Render(ID3D11DeviceContext* deviceContext);

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11ShaderResourceView* GetTextureView() const;
};

