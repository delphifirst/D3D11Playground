#include "windows_pipeline_state.h"

#include "utils.h"

namespace playground
{
	WindowsPipelineState::WindowsPipelineState(ID3D11RasterizerState* rasterizer, ID3D11DepthStencilState* depth_stencil, ID3D11BlendState* blend)
		: rasterizer_(rasterizer), depth_stencil_(depth_stencil), blend_(blend)
	{

	}

	WindowsPipelineState::~WindowsPipelineState()
	{
		SafeRelease(rasterizer_);
		SafeRelease(depth_stencil_);
		SafeRelease(blend_);
	}
}