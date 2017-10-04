#include "windows_render_target.h"

#include "utils.h"

namespace playground
{
	WindowsRenderTarget::WindowsRenderTarget(
		int width, int height,
		ID3D11RenderTargetView* render_target_view,
		ID3D11DepthStencilView* depth_stencil_view)
		: width_(width), height_(height), 
		render_target_view_(render_target_view),
		depth_stencil_view_(depth_stencil_view)
	{

	}
	WindowsRenderTarget::~WindowsRenderTarget()
	{
		SafeRelease(render_target_view_);
		SafeRelease(depth_stencil_view_);
	}
}