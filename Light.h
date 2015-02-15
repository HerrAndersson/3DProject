#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Light
{

private:

	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 direction;

public:

	Light(XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 direction);
	~Light();

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

	void SetAmbientColor(float red, float green, float blue, float alpha);
	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);

};

