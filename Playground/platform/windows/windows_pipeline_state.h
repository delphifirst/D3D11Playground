#pragma once

#include <d3d11.h>

#include "pipeline_state.h"

namespace playground
{
	class WindowsPipelineState :public IPipelineState
	{
	public:
		WindowsPipelineState(ID3D11RasterizerState* rasterizer, ID3D11DepthStencilState* depth_stencil, ID3D11BlendState* blend);
		virtual ~WindowsPipelineState() override;

		ID3D11RasterizerState* GetRasterizerState()
		{
			return rasterizer_;
		}
		ID3D11DepthStencilState* GetDepthStencilState()
		{
			return depth_stencil_;
		}
		ID3D11BlendState* GetBlendState()
		{
			return blend_;
		}
	private:
		ID3D11RasterizerState* rasterizer_;
		ID3D11DepthStencilState* depth_stencil_;
		ID3D11BlendState* blend_;
	};
}