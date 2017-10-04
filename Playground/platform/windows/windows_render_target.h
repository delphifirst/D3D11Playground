#pragma once

#include <d3d11.h>

#include "render_target.h"

namespace playground
{
	class WindowsRenderTarget :public IRenderTarget
	{
	public:
		WindowsRenderTarget(int width, int height, ID3D11RenderTargetView* render_target_view, ID3D11DepthStencilView* depth_stencil_view);
		virtual ~WindowsRenderTarget() override;
		int GetWidth() const override
		{
			return width_;
		}
		int GetHeight() const override
		{
			return height_;
		}
		ID3D11RenderTargetView* GetRenderTargetView()
		{
			return render_target_view_;
		}
		ID3D11DepthStencilView* GetDepthStencilView()
		{
			return depth_stencil_view_;
		}
	private:
		int width_;
		int height_;
		ID3D11RenderTargetView* render_target_view_ = nullptr;
		ID3D11DepthStencilView* depth_stencil_view_ = nullptr;
	};
}