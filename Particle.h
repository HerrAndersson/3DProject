#pragma once

#include <DirectXMath.h>

class Particle
{
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
	bool alive;
public:
	Particle(	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
				DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
				DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0)
			);
	~Particle();

	void Update(float frameTime);
	DirectX::XMFLOAT3 GetPosition();
	bool IsAlive();
};

