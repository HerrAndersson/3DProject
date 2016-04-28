#pragma once
#include <windows.h>
#include <stdexcept>
#include "D3DClass.h"

#include "VertexTypes.h"
#include "Camera.h"
#include "Terrain.h"
#include "Light.h"
#include "InputHandler.h"
#include "Timer.h"
#include "Position.h"

#include "Object.h"
#include "ObjectBase.h"
#include "ObjectIntersection.h"

#include "Light.h"

#include "ShaderBase.h"
#include "ShaderDefault.h"

#include "ShadowMap.h"

class Application
{

private:

	const float HEIGHT_FROM_GROUND = 5.0f;
	const int NUM_SPHERES = 5;

	int screenWidth;
	int screenHeight;

	//OBJECTS
	D3DClass*		    Direct3D;
	Camera*	            camera;
	Terrain*		    terrain;
	Position*		    position;
	InputHandler*		input;
	Timer*				timer;

	//MODELS
	ObjectBase**		spheres;
	ObjectBase*			sphere;

	//SHADERS
	ShaderDefault*		modelShader;

	//OTHER
	Light*				light;
	ShadowMap*			shadowMap;

	void HandleMovement(float frameTime);
	void CreateShaders();
	bool Render();
	bool TestIntersections(ObjectIntersection* object, float& distance);

	Ray GetRay();

public:

	Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	~Application();

	bool Update();

};


