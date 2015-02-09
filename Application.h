#pragma once
#include <windows.h>
#include <stdexcept>
#include "D3DClass.h"
#include "ShaderBase.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "Terrain.h"
#include "ShaderColor.h"

class Application
{

private:

	//OBJECTS
	D3DClass*		    Direct3D;
	Camera*	            camera;
	Terrain*		    terrain;
	//Position*		    position;
	//InputHandler*		input;

	//Only used temporary for the triangle
	ID3D11Buffer*       vertexBuffer;

	//SHADERS
	ShaderBase*			defaultShader;
	ShaderColor*        terrainShader;

	bool HandleInput(float frameTime);
	bool RenderGraphics();
	void CreateShaders();
	void CreateTriangleData();

public:
	Application(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	~Application();

	bool Update();

};


