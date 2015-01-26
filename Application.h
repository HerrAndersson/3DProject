#pragma once
#include <windows.h>
#include "D3DClass.h"
#include "ShaderBase.h"
#include "VertexTypes.h"

class Application
{

private:

	//OBJECTS. Most not implemented yet
	//InputHandler*		input;
	D3DClass*		    Direct3D;
	//Camera*	        camera;
	//Terrain*		    terrain;
	//PositionClass*	position;

	//Only used temporary for the triangle
	ID3D11Buffer*       vertexBuffer;

	//SHADERS
	ShaderBase*			defaultShader;

	bool HandleInput(float frameTime);
	bool RenderGraphics();
	bool CreateShaders(HWND hwnd);
	void CreateTriangleData();

public:
	Application();
	~Application();

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	bool Update();

};


