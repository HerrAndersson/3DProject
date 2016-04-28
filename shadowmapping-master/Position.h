#pragma once
#include <DirectXMath.h>

class Position
{
private:

	const float ACCELERATION = 0.001f;
	const float DECELERATION = 0.0007f;
	const float SPEED_MULTIPLIER = 0.03f;
	const float LOOK_SPEED = 0.5f;
	const float VIEW_BOUNDS_X = 75.0f;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;

	float frameTime;

	float forwardSpeed;
	float backwardSpeed;
	float leftSpeed;
	float rightSpeed;

public:

	Position(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 startRot);
	~Position();

	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetRotation(DirectX::XMFLOAT3 rot);
	void SetY(float y);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void SetFrameTime(float frameTime);

	void MoveForward(bool keyDown);
	void MoveBackward(bool keyDown);
	void MoveLeft(bool keyDown);
	void MoveRight(bool keyDown);

	void LookAround(DirectX::XMFLOAT2 lxly);

};

