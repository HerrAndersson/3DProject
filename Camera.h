#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>

class Camera
{

private:
	DirectX::XMFLOAT3 positionXYZ;
	DirectX::XMFLOAT3 rotationXYZ;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX baseViewMatrix;

public:
	Camera();
	~Camera();

	void SetPosition(DirectX::XMFLOAT3 newPos);
	void SetRotation(DirectX::XMFLOAT3 newRot);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void GetViewMatrix(DirectX::XMMATRIX& viewMatrix);
	void GetBaseViewMatrix(DirectX::XMMATRIX& baseViewMatrix);

	void CreateBaseViewMatrix();

	void Update();

	//Without overloading these the 16B alignment of an XMMATRIX is not guaranteed, which could possibly cause access violation
	void* operator new(size_t i);
	void operator delete(void* p);
};

