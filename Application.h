#pragma once
#include <windows.h>
#include <stdexcept>
#include "D3DClass.h"
#include "ShaderBase.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "Terrain.h"
#include "ShaderColor.h"
#include "InputHandler.h"
#include "Timer.h"
#include "Position.h"

class Application
{

private:

	const float HEIGHT_FROM_GROUND = 4.0f;

	//OBJECTS
	D3DClass*		    Direct3D;
	Camera*	            camera;
	Terrain*		    terrain;
	Position*		    position;
	InputHandler*		input;
	Timer*				timer;


	//SHADERS
	ShaderColor*        terrainShader;

	void HandleMovement(float frameTime);
	bool RenderGraphics();
	void CreateShaders();
	void CreateTriangleData();

public:
	Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	~Application();

	bool Update();

};


