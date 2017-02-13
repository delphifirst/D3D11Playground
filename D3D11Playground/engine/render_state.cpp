#include "render_state.h"

#include "utils.h"
#include "engine.h"

RenderState::RenderState()
{
	depth_stencil_desc_.DepthEnable = true;
	depth_stencil_desc_.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc_.DepthFunc = D3D11_COMPARISON_LESS;
	depth_stencil_desc_.StencilEnable = true;
	depth_stencil_desc_.StencilReadMask = 0xFF;
	depth_stencil_desc_.StencilWriteMask = 0xFF;
	depth_stencil_desc_.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc_.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc_.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc_.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc_.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc_.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc_.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc_.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	

	rasterizer_desc_.AntialiasedLineEnable = false;
	rasterizer_desc_.CullMode = D3D11_CULL_BACK;
	rasterizer_desc_.DepthBias = 0;
	rasterizer_desc_.DepthBiasClamp = 0.0f;
	rasterizer_desc_.DepthClipEnable = true;
	rasterizer_desc_.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc_.FrontCounterClockwise = false;
	rasterizer_desc_.MultisampleEnable = false;
	rasterizer_desc_.ScissorEnable = false;
	rasterizer_desc_.SlopeScaledDepthBias = 0.0f;

	blend_desc_.AlphaToCoverageEnable = false;
	blend_desc_.IndependentBlendEnable = false;
	blend_desc_.RenderTarget[0].BlendEnable = false;
	blend_desc_.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc_.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc_.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc_.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc_.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blend_desc_.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc_.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
}

void RenderState::Use()
{
	CheckForUpdate();
	Engine::Instance().device_context()->OMSetDepthStencilState(depth_stencil_state_, 1);
	Engine::Instance().device_context()->RSSetState(rasterizer_state_);
	float blend_factor[] = { 0, 0, 0, 0 };
	Engine::Instance().device_context()->OMSetBlendState(blend_state_, blend_factor, 0xffffffff);
}

void RenderState::CheckForUpdate()
{
	if (depth_stencil_updated_)
	{
		depth_stencil_updated_ = false;
		SafeRelease(depth_stencil_state_);

		HRESULT hr = Engine::Instance().device()->CreateDepthStencilState(
			&depth_stencil_desc_, &depth_stencil_state_);
		assert(SUCCEEDED(hr));
	}

	if(rasterizer_updated_)
	{
		rasterizer_updated_ = false;
		SafeRelease(rasterizer_state_);

		HRESULT hr = Engine::Instance().device()->CreateRasterizerState(
			&rasterizer_desc_, &rasterizer_state_);
		assert(SUCCEEDED(hr));
	}

	if (blend_updated_)
	{
		blend_updated_ = false;
		SafeRelease(blend_state_);

		HRESULT hr = Engine::Instance().device()->CreateBlendState(
			&blend_desc_, &blend_state_);
		assert(SUCCEEDED(hr));
	}
}

RenderState::~RenderState()
{
	SafeRelease(depth_stencil_state_);
	SafeRelease(rasterizer_state_);
	SafeRelease(blend_state_);
}