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
	ObjectIntersection(string modelFilename, string textureFilename, ID3D11Device* device, XMFLOAT3 position, XMFLOAT3 scaling);
	virtual ~ObjectIntersection();

	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 newPos);

	XMFLOAT3 GetScaling();
	void SetScaling(XMFLOAT3 newScaling);

	Sphere* GetIntersectionSphere();
	void RenderSphere(ID3D11DeviceContext* deviceContext);

	void Update();
};

