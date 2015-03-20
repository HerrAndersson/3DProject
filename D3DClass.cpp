#include "D3DClass.h"

using namespace DirectX;
using namespace std;

D3DClass::D3DClass(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{

	HRESULT result;

	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_RASTERIZER_DESC rasterNoCullingDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Create the swap chain, device, and device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
	if (FAILED(result))
	{
		throw std::runtime_error("Could not create swap buffer");
	}

	//Get the pointer to the back buffer.
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		throw std::runtime_error("Could not get swap chain pointer");
	}

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
	if (FAILED(result))
	{
		throw std::runtime_error("CreateRenderTargetView error");
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if (FAILED(result))
	{
		throw std::runtime_error("Depth buffer error");
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result))
	{
		throw std::runtime_error("Depth stencil error");
	}
	// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result))
	{
		throw std::runtime_error("Depth stencil view error");
	}

	//Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	//Setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//Create the rasterizer state from the description
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result))
	{
		throw std::runtime_error("Resterizer state error 1");
	}
	deviceContext->RSSetState(rasterState);

	rasterNoCullingDesc.AntialiasedLineEnable = false;
	rasterNoCullingDesc.CullMode = D3D11_CULL_NONE;
	rasterNoCullingDesc.DepthBias = 0;
	rasterNoCullingDesc.DepthBiasClamp = 0.0f;
	rasterNoCullingDesc.DepthClipEnable = true;
	rasterNoCullingDesc.FillMode = D3D11_FILL_SOLID;
	rasterNoCullingDesc.FrontCounterClockwise = false;
	rasterNoCullingDesc.MultisampleEnable = false;
	rasterNoCullingDesc.ScissorEnable = false;
	rasterNoCullingDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterNoCullingDesc, &rasterNoCullingState);
	if (FAILED(result))
	{
		throw std::runtime_error("Resterizer state error 2");
	}

	//Setup the viewport for rendering
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	deviceContext->RSSetViewports(1, &viewport);

	// Create the matrices for 3D rendering.
	projectionMatrix = XMMatrixPerspectiveFovLH(XM_PI * 0.45f, (float)screenWidth / (float)screenHeight, screenNear, screenDepth);
	worldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);


	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&depthDisabledStencilDesc, &depthDisabledStencilState);
	if (FAILED(result))
	{
		throw std::runtime_error("Depth stencil state error");
	}

	//Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = device->CreateBlendState(&blendStateDescription, &alphaEnableBlendingState);
	if (FAILED(result))
	{
		throw std::runtime_error("Blend state error");
	}

	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	result = device->CreateBlendState(&blendStateDescription, &alphaDisableBlendingState);
	if (FAILED(result))
	{
		throw std::runtime_error("Blend state error 2");
	}

	deferredShader = new Deferred(device, screenWidth, screenHeight);
	shadowMap = new ShadowMap(device, 4096, 4096, 0.0f);
}

D3DClass::~D3DClass()
{
	if (swapChain)
		swapChain->SetFullscreenState(false, NULL);

	if (alphaEnableBlendingState)
	{
		alphaEnableBlendingState->Release();
		alphaEnableBlendingState = nullptr;
	}

	if (alphaDisableBlendingState)
	{
		alphaDisableBlendingState->Release();
		alphaDisableBlendingState = nullptr;
	}

	if (depthDisabledStencilState)
	{
		depthDisabledStencilState->Release();
		depthDisabledStencilState = nullptr;
	}

	if (rasterState)
	{
		rasterState->Release();
		rasterState = nullptr;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}

	if (deferredShader)
	{
		delete deferredShader;
		deferredShader = nullptr;
	}

	if (shadowMap)
	{
		delete shadowMap;
		shadowMap = nullptr;
	}
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	deviceContext->ClearRenderTargetView(renderTargetView, color);

	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene()
{
	swapChain->Present(0, 0);
}

ID3D11Device* D3DClass::GetDevice()
{
	return device;
}


ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return deviceContext;
}

void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = this->projectionMatrix;
}


void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = this->worldMatrix;
}


void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = this->orthoMatrix;
}

void D3DClass::TurnZBufferON()
{
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
}

void D3DClass::TurnZBufferOFF()
{
	deviceContext->OMSetDepthStencilState(depthDisabledStencilState, 1);
}

void D3DClass::SetBackBufferRenderTarget()
{
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void D3DClass::ResetViewport()
{
	deviceContext->RSSetViewports(1, &viewport);
}

void D3DClass::TurnAlphaBlendingON()
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	deviceContext->OMSetBlendState(alphaEnableBlendingState, blendFactor, 0xffffffff);
}
void D3DClass::TurnAlphaBlendingOFF()
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	deviceContext->OMSetBlendState(alphaDisableBlendingState, blendFactor, 0xffffffff);
}

void D3DClass::TurnCullingON()
{
	deviceContext->RSSetState(rasterState);
}
void D3DClass::TurnCullingOFF()
{
	deviceContext->RSSetState(rasterNoCullingState);
}

ID3D11ShaderResourceView* D3DClass::GetDeferredSRV(int viewNumber)
{
	return deferredShader->GetShaderResourceView(viewNumber);
}

void D3DClass::ActivateDeferredShading()
{
	deferredShader->SetRenderTargets(deviceContext);
	deferredShader->ClearRenderTargets(deviceContext, 0.2f, 0.4f, 1.0f, 1.0f);
}

void D3DClass::ActivateShadowing()
{
	shadowMap->ActivateShadowing(deviceContext);
}

int D3DClass::GetShadowMapSize()
{
	return shadowMap->GetSize();
}

ID3D11ShaderResourceView* D3DClass::GetShadowMapSRV()
{
	return shadowMap->GetShadowSRV();
}

void* D3DClass::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void D3DClass::operator delete(void* p)
{
	_mm_free(p);
}

