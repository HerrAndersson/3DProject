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
};

