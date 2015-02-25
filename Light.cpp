#include "Light.h"

using namespace DirectX;

Light::Light(XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 direction)
{
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->direction = direction;
}

Light::~Light()
{
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	ambientColor = XMFLOAT4(red, green, blue, alpha);
}


void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	diffuseColor = XMFLOAT4(red, green, blue, alpha);
}


void Light::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
}


XMFLOAT4 Light::GetAmbientColor()
{
	return ambientColor;
}


XMFLOAT4 Light::GetDiffuseColor()
{
	return diffuseColor;
}


XMFLOAT3 Light::GetDirection()
{
	return direction;
}