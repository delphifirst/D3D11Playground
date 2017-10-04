#pragma once

#include <d3d11.h>

class RenderState
{
public:
	RenderState();
	~RenderState();
	RenderState(const RenderState& other) = delete;
	RenderState& operator=(const RenderState& other) = delete;

	D3D11_DEPTH_STENCIL_DESC& ChangeDepthStencilState()
	{
		depth_stencil_updated_ = true;
		return depth_stencil_desc_;
	}

	D3D11_RASTERIZER_DESC& ChangeRasterizerState()
	{
		rasterizer_updated_ = true;
		return rasterizer_desc_;
	}

	D3D11_BLEND_DESC& ChangeBlendState()
	{
		blend_updated_ = true;
		return blend_desc_;
	}

	void Use();

private:
	void CheckForUpdate();

	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc_;
	D3D11_RASTERIZER_DESC rasterizer_desc_;
	D3D11_BLEND_DESC blend_desc_;

	ID3D11DepthStencilState* depth_stencil_state_ = nullptr;
	ID3D11RasterizerState* rasterizer_state_ = nullptr;
	ID3D11BlendState* blend_state_ = nullptr;

	bool depth_stencil_updated_ = true;
	bool rasterizer_updated_ = true;
	bool blend_updated_ = true;
};