#pragma once

#include <d3d11.h>

class RenderState
{
public:
	RenderState();
	~RenderState();
	RenderState(const RenderState& other) = delete;
	RenderState& operator=(const RenderState& other) = delete;

	void Use();

private:
	ID3D11DepthStencilState* depth_stencil_state_ = nullptr;
	ID3D11RasterizerState* rasterizer_state_ = nullptr;
};