#include "ObjectBase.h"


ObjectBase::ObjectBase(ID3D11Device* device)
{
}


ObjectBase::~ObjectBase()
{
	delete vertexBuffer;
	vertexBuffer = nullptr;

	delete texture;
	texture = nullptr;
}

void ObjectBase::Render(ID3D11DeviceContext* deviceContext)
{
	//Set texture here
	deviceContext->Draw(vertexCount, 0);
}


ID3D11Buffer* ObjectBase::GetVertexBuffer() const
{
	return vertexBuffer;
}

ID3D11ShaderResourceView* ObjectBase::GetTextureView() const
{
	return texture;
}
