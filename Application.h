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
#include "OrthoWindow.h"

#include "Object.h"
#include "ObjectBase.h"
#include "ObjectIntersection.h"

#include "ParticleEmitter.h"
#include "Light.h"
#include "Quadtree.h"

#include "ShaderParticles.h"
#include "ShaderBase.h"
#include "ShaderDefault.h"
#include "ShaderTerrain.h"
#include "ShaderLight.h"

class Application
{

private:

	const float HEIGHT_FROM_GROUND = 6.0f;

	int screenWidth;
	int screenHeight;

	//OBJECTS
	D3DClass*		    Direct3D;
	Camera*	            camera;
	Terrain*		    terrain;
	Position*		    position;
	InputHandler*		input;
	Timer*				timer;
	OrthoWindow*		orthoWindow;

	//MODELS
	ObjectBase*			camel;
	ObjectBase*			wagon;
	ObjectBase*			particleEmitter;
	ObjectBase*			spheres;

	Quadtree*			modelQuadtree;

	//SHADERS
	ShaderDefault*		modelShader;
	ShaderParticles*	particleShader;
	ShaderTerrain*		terrainShader;
	ShaderLight*        lightShader;

	//OTHER
	Light* light;

	void HandleMovement(float frameTime);
	void CreateShaders();
	void RenderToTexture();
	bool RenderGraphics();
	bool TestIntersections(ObjectIntersection* object);

public:

	Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	~Application();

	bool Update();

};


