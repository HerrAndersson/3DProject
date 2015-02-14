#pragma once
#include "ObjectBase.h"
#include "Texture.h"
#include <string>
#include <vector>

class ParticleEmitter :
	public ObjectBase
{
private:
	std::vector<VertexParticles> particles;
public:
	ParticleEmitter(ID3D11Device* device, std::string textureFilename);
	virtual ~ParticleEmitter();

	
	virtual void Render(ID3D11DeviceContext* deviceContext);
	void Update(ID3D11DeviceContext* deviceContext, float frameTime);
};

