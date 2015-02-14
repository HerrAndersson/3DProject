#include "ParticleEmitter.h"

using namespace std;
using namespace DirectX;

ParticleEmitter::ParticleEmitter(ID3D11Device* device, std::string textureFilename)
{
	texture = new Texture(textureFilename, device);


	//Test particles
	VertexParticles testParticle[3];
	testParticle[0].pos = XMFLOAT3(4, 4, 0);
	testParticle[1].pos = XMFLOAT3(8, 4, 0);
	testParticle[2].pos = XMFLOAT3(12, 4, 0);
	
	particles.push_back(testParticle[0]);
	particles.push_back(testParticle[1]);
	particles.push_back(testParticle[2]);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VertexParticles) * particles.size();
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = particles.data();
	device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	vertexCount = particles.size();
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
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	deviceContext->PSSetShaderResources(0, 1, &textureView);

	deviceContext->Draw(vertexCount, 0);
}

void ParticleEmitter::Update(ID3D11DeviceContext* deviceContext, float frameTime)
{
	//Update the particles


	vertexCount = particles.size();
	//Update the buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, particles.data(), sizeof(VertexParticles)*particles.size());
	deviceContext->Unmap(vertexBuffer, 0);
}
