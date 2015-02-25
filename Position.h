#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Position
{
private:

	const float ACCELERATION = 0.001f;
	const float DECELERATION = 0.0007f;
	const float SPEED_MULTIPLIER = 0.03f;
	const float LOOK_SPEED = 0.5f;
	const float VIEW_BOUNDS_X = 75.0f;

	XMFLOAT3 position;
	XMFLOAT3 rotation;

	float frameTime;

	float forwardSpeed;
	float backwardSpeed;
	float leftSpeed;
	float rightSpeed;

public:

	Position(XMFLOAT3 startPos, XMFLOAT3 startRot);
	~Position();

	void SetPosition(XMFLOAT3 pos);
	void SetRotation(XMFLOAT3 rot);
	void SetY(float y);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void SetFrameTime(float frameTime);

	void MoveForward(bool keyDown);
	void MoveBackward(bool keyDown);
	void MoveLeft(bool keyDown);
	void MoveRight(bool keyDown);

	void LookAround(XMFLOAT2 lxly);

};

