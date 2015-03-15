#include "ObjectBase.h"

using namespace std;
using namespace DirectX;

ObjectBase::ObjectBase()
{
	/*XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());*/
}

ObjectBase::ObjectBase(DirectX::XMFLOAT4X4 worldMatrix)
{
	this->worldMatrix = worldMatrix;
}

ObjectBase::~ObjectBase()
{
	if (vertexBuffer)
		vertexBuffer->Release();
	delete texture;
}

void ObjectBase::Update(ID3D11DeviceContext* deviceContext, float frameTime)
{

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
	worldMatrix = this->worldMatrix;
}

void* ObjectBase::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void ObjectBase::operator delete(void* p)
{
	_mm_free(p);
}
