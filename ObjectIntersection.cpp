#include "ObjectIntersection.h"

ObjectIntersection::ObjectIntersection(ID3D11Device* device, string modelFilename, string textureFilename, XMFLOAT3 position, XMFLOAT3 scaling, XMMATRIX& world)
				  : Object(device, modelFilename, textureFilename, world)
{
	this->position = position;
	this->scaling = scaling;
	worldMatrix = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixTranslation(position.x, position.y, position.z);
	updateWorld = false;

	intersectionSphere = new Sphere(position, scaling.x, device, worldMatrix);
}


ObjectIntersection::~ObjectIntersection()
{
	delete intersectionSphere;
	intersectionSphere = nullptr;
}

//This could be called only when SET functions are called, but if there are objects with continuous movement it has to be called each frame
void ObjectIntersection::Update()
{
	if (updateWorld)
	{
		worldMatrix = XMMatrixScaling(scaling.x, scaling.y, scaling.z)*XMMatrixTranslation(position.x, position.y, position.z);
		intersectionSphere->center = position;
		updateWorld = false;
	}
	
	if (position.y > 5)
	{
		position.y--;
		updateWorld = true;
	}
}

XMFLOAT3 ObjectIntersection::GetPosition()
{
	return position;
}
void ObjectIntersection::SetPosition(XMFLOAT3 newPos)
{
	position = newPos;
	updateWorld = true;
}

XMFLOAT3 ObjectIntersection::GetScaling()
{
	return scaling;
}
void ObjectIntersection::SetScaling(XMFLOAT3 newScaling)
{
	scaling = newScaling;
	updateWorld = true;
}

Sphere* ObjectIntersection::GetIntersectionSphere()
{
	return intersectionSphere;
}

void ObjectIntersection::RenderSphere(ID3D11DeviceContext* deviceContext)
{
	intersectionSphere->Render(deviceContext);
}


