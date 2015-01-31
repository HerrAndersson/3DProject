#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::SetPosition(XMFLOAT3 newPos)
{
	positionXYZ = newPos;
}
void Camera::SetRotation(XMFLOAT3 newRot)
{
	rotationXYZ = newRot;
}

XMFLOAT3 Camera::GetPosition()
{
	return positionXYZ;
}
XMFLOAT3 Camera::GetRotation()
{
	return rotationXYZ;
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
}

void Camera::Update()
{
	XMMATRIX rotationMatrix;

	XMVECTOR up = { { 0.0f, 1.0f, 0.0f } };
	XMVECTOR position = { { positionXYZ.x, positionXYZ.y, positionXYZ.z } };
	XMVECTOR lookAt = { { 0.0f, 0.0f, 1.0f } };

	rotationMatrix = XMMatrixRotationRollPitchYaw(rotationXYZ.x * RAD, rotationXYZ.y * RAD, rotationXYZ.z * RAD);

	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	lookAt = position + lookAt;
	viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}
