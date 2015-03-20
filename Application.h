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
#include "ShadowLight.h"

#include "ShaderParticles.h"
#include "ShaderBase.h"
#include "ShaderDefault.h"
#include "ShaderTerrain.h"
#include "ShaderLight.h"
#include "ShaderShadowMap.h"

class Application
{

private:

	const float HEIGHT_FROM_GROUND = 6.0f;
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
	OrthoWindow*		orthoWindow;

	//MODELS
	ObjectBase*			camel;
	ObjectBase*			wagon;
	ObjectBase*			particleEmitter;
	ObjectBase**		spheres;
	ObjectBase*			sphere;

	Quadtree*			modelQuadtree;

	//SHADERS
	ShaderDefault*		modelShader;
	ShaderParticles*	particleShader;
	ShaderTerrain*		terrainShader;
	ShaderLight*        lightShader;
	ShaderShadowMap*    shadowMapShader;

	//OTHER
	Light*				light;
	ShadowLight*        shadowLight;

	void HandleMovement(float frameTime);
	void CreateShaders();
	void RenderToTexture();
	bool RenderGraphics();
	bool TestIntersections(ObjectIntersection* object, float& distance);

	Ray GetRay();

public:

	Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	~Application();

	bool Update();

};


