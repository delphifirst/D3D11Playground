#pragma once

#include <string>
#include <vector>

#include <Windows.h>
#include <d3d11.h>

#include "camera_object.h"

class Object;

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
	// Init other staffs
	void InitEngine();
	void Dispose();

	void AddTopLevelObject(Object* object);

	int height() const
	{
		return height_;
	}

	int width() const
	{
		return width_;
	}

	ID3D11Device* device()
	{
		return d3d_device_;
	}

	ID3D11DeviceContext* device_context()
	{
		return d3d_device_context_;
	}

	CameraObject* main_camera()
	{
		return main_camera_;
	}

	void Update(double delta_time);
	void DrawScene();

	void OnTick(double delta_time);
	void OnResizeWindow(int width, int height);

private:
	Engine() = default;

	int width_, height_;

	// Members for Direct3D stuff
	ID3D11Device * d3d_device_;
	ID3D11DeviceContext * d3d_device_context_;
	IDXGISwapChain* swap_chain_ = nullptr;
	ID3D11RenderTargetView* render_target_view_ = nullptr;
	ID3D11DepthStencilView* depth_stencil_view_ = nullptr;

	std::vector<Object*> top_level_objects_;
	CameraObject * main_camera_;
};

const DXGI_FORMAT kBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;