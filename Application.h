#pragma once
#include <windows.h>
#include <stdexcept>
#include "D3DClass.h"
#include "ShaderBase.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "Terrain.h"
#include "ShaderTerrain.h"
#include "InputHandler.h"
#include "Timer.h"
#include "Position.h"
#include "ShaderDefault.h"
#include "Object.h"
#include "ShaderParticles.h"
#include "ParticleEmitter.h"
#include "Light.h"
#include "Quadtree.h"

//Deferred shading
#include "ShaderLight.h"
#include "OrthoWindow.h"

class Application
{

private:

	const float HEIGHT_FROM_GROUND = 6.0f;

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
	ObjectBase*			sphere;

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

public:

	Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	~Application();

	bool Update();

};


