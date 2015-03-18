#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class ShadowLight
{

private:

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 position;
	XMFLOAT3 lookAt;

public:

	ShadowLight();
	ShadowLight(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT3 position, XMFLOAT3 lookAt);
	~ShadowLight();

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetPosition();

	void SetAmbientColor(XMFLOAT4 ambient);
	void SetDiffuseColor(XMFLOAT4 diffuse);
	void SetPosition(XMFLOAT3 position);
	void SetLookAt(XMFLOAT3 lookAt);

	void CreateViewMatrix();
	void CreateProjectionMatrix(float screenDepth, float screenNear);

	void GetViewMatrix(XMMATRIX& viewMatrix);
	void GetProjectionMatrix(XMMATRIX& projectionMatrix);

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);
};




