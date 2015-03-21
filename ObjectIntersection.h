#pragma once
#include "Object.h"
#include <DirectXMath.h>
#include "Intersections.h"

using namespace DirectX;
using namespace std;

class ObjectIntersection : public Object
{
private:

	XMFLOAT3 position;
	XMFLOAT3 scaling;

	Sphere* intersectionSphere;

	bool updateWorld;

public:

	ObjectIntersection();
	ObjectIntersection(ID3D11Device* device, string modelFilename, XMFLOAT3 position, XMFLOAT3 scaling, XMMATRIX& world);
	virtual ~ObjectIntersection();

	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 newPos);

	XMFLOAT3 GetScaling();
	void SetScaling(XMFLOAT3 newScaling);

	Sphere* GetIntersectionSphere();

	void Update();
};

