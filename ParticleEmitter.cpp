#include "ParticleEmitter.h"

using namespace std;
using namespace DirectX;

ParticleEmitter::ParticleEmitter(ID3D11Device* device, std::string textureFilename)
{
	texture = new Texture(textureFilename, device);
	particles.resize(100);

	VertexParticles testParticle;
	testParticle.pos = XMFLOAT3(0, 4, 0);

	particles.push_back(testParticle);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VertexParticles) * particles.size();
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = particles.data();
	device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	vertexCount = particles.size() * 2;
}


ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Render(ID3D11DeviceContext* deviceContext)
{
	UINT32 vertexSize = sizeof(VertexParticles);
	UINT32 offset = 0;
	ID3D11ShaderResourceView* textureView = GetTexture();

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->PSSetShaderResources(0, 1, &textureView);

	deviceContext->Draw(vertexCount, 0);
}

void ParticleEmitter::Update(ID3D11DeviceContext* deviceContext, float frameTime)
{
	//Update the particles


	vertexCount = particles.size() * 2;
	//Update the buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, particles.data(), sizeof(VertexParticles)*particles.size());
	deviceContext->Unmap(vertexBuffer, 0);
}
