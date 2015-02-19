#include "ParticleEmitter.h"

using namespace std;
using namespace DirectX;

ParticleEmitter::ParticleEmitter(ID3D11Device* device, std::string textureFilename)
{
	texture = new Texture(textureFilename, device);

	//Test particles
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			particles.push_back(new Particle(XMFLOAT3(2*i, rand()%100+1, 2*j), XMFLOAT3(0, -50, 0), XMFLOAT3(0, 0, 0)));
		}
	}
	
	particleData.clear();
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		particleData.push_back(particles.at(i)->GetPosition());
	}

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(XMFLOAT3) * particleData.size();
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = particleData.data();
	device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	vertexCount = particleData.size();
}


ParticleEmitter::~ParticleEmitter()
{
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		delete particles.at(i);
	}
}

void ParticleEmitter::Render(ID3D11DeviceContext* deviceContext)
{
	UINT32 vertexSize = sizeof(XMFLOAT3);
	UINT32 offset = 0;
	ID3D11ShaderResourceView* textureView = GetTexture();

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	deviceContext->PSSetShaderResources(0, 1, &textureView);

	deviceContext->Draw(vertexCount, 0);
}

void ParticleEmitter::Update(ID3D11DeviceContext* deviceContext, float frameTime)
{
	//Update the particleData
	particleData.clear();
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		Particle* currentParticle = particles.at(i);
		currentParticle->Update(frameTime);
		if (!currentParticle->IsAlive())
		{
			XMFLOAT3 oldPosition = currentParticle->GetPosition();
			delete currentParticle;
			particles[i] = new Particle(XMFLOAT3(oldPosition.x, rand() % 100 + 100, oldPosition.z), XMFLOAT3(0, -50, 0), XMFLOAT3(0, 0, 0));
		}
		
		particleData.push_back(currentParticle->GetPosition());
	}

	vertexCount = particleData.size();
	//Update the buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, particleData.data(), sizeof(XMFLOAT3)*particleData.size());
	deviceContext->Unmap(vertexBuffer, 0);
}
