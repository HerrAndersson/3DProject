#include "Particle.h"

using namespace DirectX;

Particle::Particle(XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 acceleration)
{
	this->position = position;
	this->velocity = velocity;
	this->acceleration = acceleration;
}

Particle::~Particle()
{
}

void Particle::Update(float frameTime)
{
	float factor = frameTime / 1000;

	XMVECTOR tempPosition = XMLoadFloat3(&position);
	XMVECTOR tempVelocity = XMLoadFloat3(&velocity);
	XMVECTOR tempAcceleration = XMLoadFloat3(&acceleration);

	tempVelocity += tempAcceleration * factor;
	tempPosition += tempVelocity * factor;

	XMStoreFloat3(&position, tempPosition);
	XMStoreFloat3(&velocity, tempVelocity);
	XMStoreFloat3(&acceleration, tempAcceleration);

	alive = position.y > -10;
}

XMFLOAT3 Particle::GetPosition()
{
	return position;
}

bool Particle::IsAlive()
{
	return alive;
}
