#pragma once
#include <windows.h>
#include "D3DClass.h"
#include "Vertex.h"

class Application
{

private:

	//OBJECTS. Most not implemented yet
	//InputHandler*		input;
	D3DClass*		    Direct3D;
	//Camera*	        camera;
	//Terrain*		    terrain;
	//PositionClass*	position;

	//SHADERS
	ID3D11VertexShader*    defaultVertexShader = nullptr;
	ID3D11InputLayout*     defaultVertexLayout = nullptr;
	ID3D11PixelShader*     defaultPixelShader = nullptr;
	ID3D11Buffer*          vertexBuffer = nullptr;

	bool HandleInput(float frameTime);
	bool RenderGraphics();
	void CreateShaders();

public:
	Application();
	~Application();

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Update();

};


