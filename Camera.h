#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>

using namespace DirectX;

class Camera
{

private:

	const float RAD = 0.0174532925f;

	XMFLOAT3 positionXYZ;
	XMFLOAT3 rotationXYZ;
	XMMATRIX viewMatrix;

public:
	Camera();
	~Camera();

	void SetPosition(XMFLOAT3 newPos);
	void SetRotation(XMFLOAT3 newRot);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void GetViewMatrix(XMMATRIX& viewMatrix);

	void Update();

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);
};

