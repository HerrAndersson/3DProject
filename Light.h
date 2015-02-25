#pragma once
#include <DirectXMath.h>

class Light
{

private:

	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 direction;

public:

	Light(DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT3 direction);
	~Light();

	DirectX::XMFLOAT4 GetAmbientColor();
	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT3 GetDirection();

	void SetAmbientColor(float red, float green, float blue, float alpha);
	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);

};

