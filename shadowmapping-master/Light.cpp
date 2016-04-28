#include "Light.h"

using namespace DirectX;

Light::Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT3 position, XMFLOAT3 lookAt, float screenDepth, float screenNear)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->position = position;
	this->lookAt = lookAt;

	CreateViewMatrix();
	CreateProjectionMatrix(screenDepth, screenNear);
}

Light::~Light()
{

}

XMFLOAT4 Light::GetAmbientColor()
{
	return ambient;
}

XMFLOAT4 Light::GetDiffuseColor()
{
	return diffuse;
}

XMFLOAT3 Light::GetPosition()
{
	return position;
}

void Light::CreateViewMatrix()
{
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 0.0f);
	XMVECTOR look = XMVectorSet(lookAt.x, lookAt.y, lookAt.z, 0.0f);

	viewMatrix = XMMatrixLookAtLH(pos, look, up);
}

void Light::CreateProjectionMatrix(float screenDepth, float screenNear)
{
	//Field of view and screen aspect for square light
	projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0f, screenNear, screenDepth);
}

void Light::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
}

void Light::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = this->projectionMatrix;
}

XMFLOAT3 Light::GetDirection()
{
	XMFLOAT3 dir(lookAt.x - position.x, lookAt.y - position.y, lookAt.z - position.z);

	float length = sqrt((dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z));

	dir.x = dir.x / length;
	dir.y = dir.y / length;
	dir.z = dir.z / length;

	return dir;
}

void* Light::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void Light::operator delete(void* p)
{
	_mm_free(p);
}