#include "ObjectBase.h"

using namespace std;

ObjectBase::ObjectBase()
{

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
