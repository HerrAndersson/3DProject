#include "ObjectBase.h"


ObjectBase::ObjectBase(ID3D11Device* device, string textureFilename)
{
	//Create shader resource view, texture.

	//D3D11_BUFFER_DESC bufferDesc;
	//memset(&bufferDesc, 0, sizeof(bufferDesc));
	//bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//bufferDesc.ByteWidth = sizeof(triangleVertices);

	//D3D11_SUBRESOURCE_DATA data;
	//data.pSysMem = triangleVertices;
	//gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
}


ObjectBase::~ObjectBase()
{
	//vertexBuffer->Release(); //Uncomment when object get its own buffer. Using global for now
	//texture->Release();
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
