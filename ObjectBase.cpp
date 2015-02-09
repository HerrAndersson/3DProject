#include "ObjectBase.h"

using namespace std;

ObjectBase::ObjectBase()
{

}

ObjectBase::~ObjectBase()
{
	vertexBuffer->Release();

	delete texture;
	texture = nullptr;
}

void ObjectBase::Render(ID3D11DeviceContext* deviceContext)
{
	UINT32 vertexSize = sizeof(float) * 6;
	UINT32 offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	ID3D11ShaderResourceView* textureView = GetTexture();
	deviceContext->PSSetShaderResources(0, 1, &textureView);

	deviceContext->Draw(vertexCount, 0);
}


ID3D11Buffer* ObjectBase::GetVertexBuffer() const
{
	return vertexBuffer;
}

ID3D11ShaderResourceView* ObjectBase::GetTexture() const
{
	return texture->GetTexture();
}
