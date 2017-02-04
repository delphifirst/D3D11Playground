#pragma once

#include <string>

#include <Windows.h>
#include <d3d11.h>

class Engine
{
public:
	// Singleton
	static Engine& Instance()
	{
		static Engine instance;
		return instance;
	}
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	// Init d3d device, device context and swap chain
	void Init(HWND main_hwnd, int width, int height);
	// Init render target view and depth stencil view.
	// This function can be called to resize render target.
	void InitRenderTarget(int width, int height);
	void Dispose();

	void OnTick(double delta_time);
	void OnResizeWindow(int width, int height);

private:
	Engine() = default;

	// Members for Direct3D stuff
	ID3D11Device * d3d_device_;
	ID3D11DeviceContext * d3d_device_context_;
	IDXGISwapChain* swap_chain_ = nullptr;
	ID3D11RenderTargetView* render_target_view_ = nullptr;
	ID3D11DepthStencilView* depth_stencil_view_ = nullptr;
};

const DXGI_FORMAT kBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;