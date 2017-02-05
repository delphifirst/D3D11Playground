#include "engine.h"

#include <DirectXMath.h>

#include "object.h"
#include "utils.h"

using namespace DirectX;

void Engine::Init(HWND main_hwnd, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = width;
	swap_chain_desc.BufferDesc.Height = height;
	swap_chain_desc.BufferDesc.Format = kBackBufferFormat;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = main_hwnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = true;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;

	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	AssertSucceed(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0,
		&feature_level, 1, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain_,
		&d3d_device_, nullptr, &d3d_device_context_),
		L"Cannot create device and swap chain");

	InitRenderTarget(width, height);
	InitEngine();
}

void Engine::InitRenderTarget(int width, int height)
{
	width_ = width;
	height_ = height;

	// Release previous render target view and depth stencil view
	SafeRelease(render_target_view_);
	SafeRelease(depth_stencil_view_);

	// Resize the swap chain and recreate the render target view.
	AssertSucceed(swap_chain_->ResizeBuffers(1, width, height,
		kBackBufferFormat, 0), L"Resize swap chain error");

	// Create render target view
	ID3D11Texture2D* back_buffer;
	swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&back_buffer));
	d3d_device_->CreateRenderTargetView(back_buffer, 0, &render_target_view_);
	SafeRelease(back_buffer);

	// Create depth/stencil buffer and view
	D3D11_TEXTURE2D_DESC depth_stencil_desc;
	depth_stencil_desc.Width = width;
	depth_stencil_desc.Height = height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_desc.SampleDesc.Count = 1;
	depth_stencil_desc.SampleDesc.Quality = 0;
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;
	ID3D11Texture2D* depth_stencil_buffer;
	d3d_device_->CreateTexture2D(&depth_stencil_desc, 0,
		&depth_stencil_buffer);
	d3d_device_->CreateDepthStencilView(depth_stencil_buffer, 0,
		&depth_stencil_view_);
	SafeRelease(depth_stencil_buffer);

	// bind render target view and depth/stencil view
	d3d_device_context_->OMSetRenderTargets(1, &render_target_view_,
		depth_stencil_view_);

	// Set viewport
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	d3d_device_context_->RSSetViewports(1, &vp);
}

void Engine::InitEngine()
{
	main_camera_ = new CameraObject(L"main_camera", XM_PI / 3, 0.3, 1000);
	AddTopLevelObject(main_camera_);
	main_camera_->position.z = -5;
	main_camera_->position.y = 1;
}

void Engine::AddTopLevelObject(Object* object)
{
	top_level_objects_.push_back(object);
}

void Engine::Dispose()
{
	for (Object* object : top_level_objects_)
	{
		delete object;
	}
	SafeRelease(swap_chain_);
	SafeRelease(d3d_device_);
	SafeRelease(d3d_device_context_);
	SafeRelease(render_target_view_);
	SafeRelease(depth_stencil_view_);
}

void Engine::OnResizeWindow(int width, int height)
{
	InitRenderTarget(width, height);
}

void Engine::OnTick(double delta_time)
{
	float clear_color[] = { 0, 0, 0, 1 };
	d3d_device_context_->ClearRenderTargetView(render_target_view_, clear_color);
	d3d_device_context_->ClearDepthStencilView(depth_stencil_view_,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	Update(delta_time);
	DrawScene();

	swap_chain_->Present(1, 0);
}

void Engine::Update(double delta_time)
{
	for (Object* object : top_level_objects_)
	{
		object->OnUpdate(delta_time);
		object->UpdateWorldTransform(XMMatrixIdentity());
	}
}

void Engine::DrawScene()
{
	for (Object* object : top_level_objects_)
	{
		object->OnDraw();
	}
}