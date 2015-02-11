#include "Position.h"
#include <iostream>
using namespace std;


Position::Position(XMFLOAT3 startPos, XMFLOAT3 startRot)
{
	position = startPos;
	rotation = startRot;

	frameTime = 0;

	forwardSpeed = 0;
	backwardSpeed = 0;
	leftSpeed = 0;
	rightSpeed = 0;
}

Position::~Position()
{
}

void Position::SetPosition(XMFLOAT3 pos)
{
	position = pos;
}
void Position::SetRotation(XMFLOAT3 rot)
{
	rotation = rot;
}

void Position::SetY(float y)
{
	position.y = y;
}

XMFLOAT3 Position::GetPosition()
{
	return position;
}
XMFLOAT3 Position::GetRotation()
{
	return rotation;
}

void Position::SetFrameTime(float frameTime)
{
	this->frameTime = frameTime;
}

void Position::LookAround(XMFLOAT2 lxly)
{
	if (lxly.x < 1000000.0f && lxly.y < 1000000.0f)
	{
		rotation.y += lxly.x * LOOK_SPEED;
		rotation.x += lxly.y * LOOK_SPEED;

		if (rotation.y < 0.0f)
		{
			rotation.y += 360.0f;
		}
		else if (rotation.y > 360.0f)
		{
			rotation.y = 0;
		}

		if (rotation.x < -45.0f)
		{
			rotation.x = -45.0f;
		}
		else if (rotation.x > 45.0f)
		{
			rotation.x = 45.0f;
		}
	}
}

void Position::MoveForward(bool keyDown)
{
	//Update the forward speed movement. If the key is down, accelerate.
	if (keyDown)
	{
		forwardSpeed += frameTime * ACCELERATION;
		if (forwardSpeed > (frameTime * SPEED_MULTIPLIER))
		{
			forwardSpeed = frameTime * SPEED_MULTIPLIER;
		}
	}
	else
	{
		forwardSpeed -= frameTime * DECELERATION;
		if (forwardSpeed < 0.0f)
		{
			forwardSpeed = 0.0f;
		}
	}

	float radians = rotation.y * RAD;

	//Update the position.
	position.x += sinf(radians) * forwardSpeed;
	position.z += cosf(radians) * forwardSpeed;
}

void Position::MoveBackward(bool keyDown)
{
	//Update the forward speed movement. If the key is down, accelerate.
	if (keyDown)
	{
		backwardSpeed += frameTime * ACCELERATION;
		if (backwardSpeed > (frameTime * SPEED_MULTIPLIER))
		{
			backwardSpeed = frameTime * SPEED_MULTIPLIER;
		}
	}
	else
	{
		backwardSpeed -= frameTime * DECELERATION;
		if (backwardSpeed < 0.0f)
		{
			backwardSpeed = 0.0f;
		}
	}

	float radians = rotation.y * RAD;

	//Update the position.
	position.x -= sinf(radians) * backwardSpeed;
	position.z -= cosf(radians) * backwardSpeed;
}

void Position::MoveLeft(bool keyDown)
{
	//Update the forward speed movement. If the key is down, accelerate.
	if (keyDown)
	{
		leftSpeed += frameTime * ACCELERATION;
		if (leftSpeed > (frameTime * SPEED_MULTIPLIER))
		{
			leftSpeed = frameTime * SPEED_MULTIPLIER;
		}
	}
	else
	{
		leftSpeed -= frameTime * DECELERATION;
		if (leftSpeed < 0.0f)
		{
			leftSpeed = 0.0f;
		}
	}

	float radians = rotation.y * RAD;

	//Update the position.
	position.x += sinf(radians) * leftSpeed;
	position.z += cosf(radians) * leftSpeed;
}

void Position::MoveRight(bool keyDown)
{

}