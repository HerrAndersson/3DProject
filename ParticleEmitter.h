#pragma once
#include "ObjectBase.h"
#include "Texture.h"
#include "Particle.h"
#include <string>
#include <vector>

class ParticleEmitter : public ObjectBase
{
private:
	std::vector<DirectX::XMFLOAT3> particleData;
	std::vector<Particle*> particles;
public:
	ParticleEmitter(ID3D11Device* device, std::string textureFilename);
	virtual ~ParticleEmitter();

	virtual void Render(ID3D11DeviceContext* deviceContext);
	virtual void Update(ID3D11DeviceContext* deviceContext, float frameTime);
};

