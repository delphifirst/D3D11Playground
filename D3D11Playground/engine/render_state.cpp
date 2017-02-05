#include "render_state.h"

#include "utils.h"
#include "engine.h"

RenderState::RenderState()
{
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	// Initialize the description of the stencil state.
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

	// Set up the description of the stencil state.
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	AssertSucceed(Engine::Instance().device()->CreateDepthStencilState(
		&depth_stencil_desc, &depth_stencil_state_), L"Cannot create depth stencil state");

	D3D11_RASTERIZER_DESC rasterizer_desc;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterizer_desc.AntialiasedLineEnable = false;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0.0f;
	rasterizer_desc.DepthClipEnable = true;
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.FrontCounterClockwise = false;
	rasterizer_desc.MultisampleEnable = false;
	rasterizer_desc.ScissorEnable = false;
	rasterizer_desc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	AssertSucceed(Engine::Instance().device()->CreateRasterizerState(
		&rasterizer_desc, &rasterizer_state_), L"Cannot create depth stencil state");
}

void RenderState::Use()
{
	// Set the depth stencil state.
	Engine::Instance().device_context()->OMSetDepthStencilState(depth_stencil_state_, 1);
	// Now set the rasterizer state.
	Engine::Instance().device_context()->RSSetState(rasterizer_state_);
}

RenderState::~RenderState()
{
	SafeRelease(rasterizer_state_);
	SafeRelease(depth_stencil_state_);
}