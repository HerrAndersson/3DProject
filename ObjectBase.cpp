#include "ObjectBase.h"

using namespace std;
using namespace DirectX;

ObjectBase::ObjectBase()
{
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
}

ObjectBase::~ObjectBase()
{
	if (vertexBuffer)
		vertexBuffer->Release();
	delete texture;
}


ID3D11Buffer* ObjectBase::GetVertexBuffer() const
{
	return vertexBuffer;
}

ID3D11ShaderResourceView* ObjectBase::GetTexture() const
{
	return texture->GetTexture();
}

void ObjectBase::GetWorldMatrix(XMMATRIX& worldMatrix) const
{
	worldMatrix = XMLoadFloat4x4(&this->worldMatrix);
}
