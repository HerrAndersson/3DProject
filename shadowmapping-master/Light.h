#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Light
{

private:

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT3 position;
	XMFLOAT3 lookAt;

public:

	Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT3 position, XMFLOAT3 lookAt, float screenDepth, float screenNear);
	~Light();

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetPosition();

	void CreateViewMatrix();
	void CreateProjectionMatrix(float screenDepth, float screenNear);

	void GetViewMatrix(XMMATRIX& viewMatrix);
	void GetProjectionMatrix(XMMATRIX& projectionMatrix);
	XMFLOAT3 GetDirection();

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);

};

