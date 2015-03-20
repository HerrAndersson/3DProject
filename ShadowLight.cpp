#include "ShadowLight.h"

ShadowLight::ShadowLight()
{
	//If default constructed object should be used, then all values have to be set by the SET functions.
}

ShadowLight::ShadowLight(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT3 position, XMFLOAT3 lookAt)
{
	ambientColor = ambient;
	diffuseColor = diffuse;
	this->position = position;
	this->lookAt = lookAt;
}

ShadowLight::~ShadowLight()
{

}

XMFLOAT4 ShadowLight::GetAmbientColor()
{
	return ambientColor;
}

XMFLOAT4 ShadowLight::GetDiffuseColor()
{
	return diffuseColor;
}

XMFLOAT3 ShadowLight::GetPosition()
{
	return position;
}

void ShadowLight::SetAmbientColor(XMFLOAT4 ambient)
{
	ambientColor = ambient;
}
void ShadowLight::SetDiffuseColor(XMFLOAT4 diffuse)
{
	diffuseColor = diffuse;
}

void ShadowLight::SetPosition(XMFLOAT3 position)
{
	this->position = position;
}

void ShadowLight::SetLookAt(XMFLOAT3 lookAt)
{
	this->lookAt = lookAt;
}

void ShadowLight::CreateViewMatrix()
{
	XMVECTOR up = { { 0.0f, 1.0f, 0.0f } };
	XMVECTOR pos = { { position.x, position.y, position.z } };
	XMVECTOR look = { { lookAt.x, lookAt.y, lookAt.z } };

	viewMatrix = XMMatrixLookAtLH(pos, look, up);
}

void ShadowLight::CreateProjectionMatrix(float screenDepth, float screenNear)
{
	//Field of view and screen aspect for square light
	float fov = (float)XM_PI / 2.0f;
	float screenAspect = 1.0f;

	projectionMatrix = XMMatrixPerspectiveFovLH(fov, screenAspect, screenNear, screenDepth);
}

void ShadowLight::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
}

void ShadowLight::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = this->projectionMatrix;
}

XMFLOAT3 ShadowLight::GetDirection()
{
	XMFLOAT3 dir(lookAt.x - position.x, lookAt.y - position.y, lookAt.z - position.z);

	float length = sqrt((dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z));

	dir.x = dir.x / length;
	dir.y = dir.y / length;
	dir.z = dir.z / length;
	
	return dir;
}

void* ShadowLight::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void ShadowLight::operator delete(void* p)
{
	_mm_free(p);
}
