#include "windows_device.h"

#include <memory>
#include <cassert>

#include <d3d11.h>

#include "utils.h"
#include "displayer.h"
#include "windows_buffer.h"
#include "windows_constant.h"
#include "windows_input_layout.h"
#include "windows_shader.h"
#include "windows_pipeline_state.h"

using namespace std;

namespace playground
{
	shared_ptr<IDevice> CreateDevice()
	{
		return make_shared<WindowsDevice>();
	}

	void WindowsDevice::Init(shared_ptr<IDisplayer> displayer)
	{
		SetupConstantMap();

		displayer_ = displayer;

		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.BufferDesc.Width = displayer->GetHeight();
		swap_chain_desc.BufferDesc.Height = displayer->GetWidth();
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = static_cast<HWND>(displayer->GetHandle());
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.Windowed = true;
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.Flags = 0;

		D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
		UINT flags = 0;
#ifndef NDEBUG
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif
		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flags,
			&feature_level, 1, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain_,
			&device_, nullptr, &device_context_);
		assert(SUCCEEDED(hr));

		CreateDefaultRenderTarget(displayer->GetWidth(), displayer->GetHeight());
		SetRenderTarget(default_render_target_);
	}

	void WindowsDevice::Fini()
	{
		SafeRelease(swap_chain_);
		SafeRelease(device_context_);
		SafeRelease(device_);
	}

	void WindowsDevice::CreateDefaultRenderTarget(int width, int height)
	{
		HRESULT hr = swap_chain_->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0);
		assert(SUCCEEDED(hr));

		ID3D11RenderTargetView* render_target_view;
		ID3D11DepthStencilView* depth_stencil_view;

		// Create render target view
		ID3D11Texture2D* back_buffer;
		swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
		device_->CreateRenderTargetView(back_buffer, 0, &render_target_view);
		SafeRelease(back_buffer);

		// Create depth/stencil buffer and view
		D3D11_TEXTURE2D_DESC depth_stencil_desc;
		depth_stencil_desc.Width = width;
		depth_stencil_desc.Height = height;
		depth_stencil_desc.MipLevels = 1;
		depth_stencil_desc.ArraySize = 1;
		depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_desc.SampleDesc.Count = 1;
		depth_stencil_desc.SampleDesc.Quality = 0;
		depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_desc.CPUAccessFlags = 0;
		depth_stencil_desc.MiscFlags = 0;
		ID3D11Texture2D* depth_stencil_buffer;
		device_->CreateTexture2D(&depth_stencil_desc, 0, &depth_stencil_buffer);
		device_->CreateDepthStencilView(depth_stencil_buffer, 0, &depth_stencil_view);
		SafeRelease(depth_stencil_buffer);

		default_render_target_ = make_shared<WindowsRenderTarget>(width, height, render_target_view, depth_stencil_view);
	}

	shared_ptr<IVertexBuffer> WindowsDevice::CreateVertexBuffer(void* init_data, int length)
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = length;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		ID3D11Buffer* buffer;
		D3D11_SUBRESOURCE_DATA subres_data;
		subres_data.pSysMem = init_data;
		HRESULT hr = device_->CreateBuffer(&desc, init_data ? &subres_data : nullptr, &buffer);
		assert(SUCCEEDED(hr));

		return make_shared<WindowsVertexBuffer>(length, buffer);
	}

	shared_ptr<IInputLayout> WindowsDevice::CreateInputLayout(
		const vector<InputElementDesc>& input_desc, shared_ptr<IShader> vertex_shader)
	{
		shared_ptr<WindowsShader<ID3D11VertexShader>> shader = static_pointer_cast<WindowsShader<ID3D11VertexShader>>(vertex_shader);
		vector<D3D11_INPUT_ELEMENT_DESC> desc(input_desc.size());
		for (int i = 0; i < desc.size(); ++i)
		{
			desc[i].SemanticName = input_desc[i].name.c_str();
			desc[i].SemanticIndex = 0;
			desc[i].Format = static_cast<DXGI_FORMAT>(gFormatMap[input_desc[i].format]);
			desc[i].InputSlot = input_desc[i].slot;
			desc[i].AlignedByteOffset = input_desc[i].byte_offset;
			desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc[i].InstanceDataStepRate = 0;
		}
		ID3D11InputLayout* input_layout;
		HRESULT hr = device_->CreateInputLayout(desc.data(), desc.size(), shader->GetCode().data(), shader->GetCode().size(), &input_layout);
		assert(SUCCEEDED(hr));

		return make_shared<WindowsInputLayout>(input_layout);
	}

	shared_ptr<IShader> WindowsDevice::CreateVertexShader(const string& name, const std::vector<char>& code)
	{
		ID3D11VertexShader* shader;
		HRESULT hr = device_->CreateVertexShader(code.data(), code.size(), nullptr, &shader);
		assert(SUCCEEDED(hr));
		
		return make_shared<WindowsShader<ID3D11VertexShader>>(name, code, shader);
	}

	shared_ptr<IShader> WindowsDevice::CreatePixelShader(const string& name, const std::vector<char>& code)
	{
		ID3D11PixelShader* shader;
		HRESULT hr = device_->CreatePixelShader(code.data(), code.size(), nullptr, &shader);
		assert(SUCCEEDED(hr));

		return make_shared<WindowsShader<ID3D11PixelShader>>(name, code, shader);
	}

	shared_ptr<IPipelineState> WindowsDevice::CreatePipelineState()
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
		depth_stencil_desc.DepthEnable = true;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
		depth_stencil_desc.StencilEnable = false;
		depth_stencil_desc.StencilReadMask = 0xFF;
		depth_stencil_desc.StencilWriteMask = 0xFF;
		depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		ID3D11DepthStencilState* depth_stencil;
		HRESULT hr = device_->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil);
		assert(SUCCEEDED(hr));

		D3D11_RASTERIZER_DESC rasterizer_desc;
		rasterizer_desc.AntialiasedLineEnable = false;
		rasterizer_desc.CullMode = D3D11_CULL_BACK;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0.0f;
		rasterizer_desc.DepthClipEnable = true;
		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.FrontCounterClockwise = true;
		rasterizer_desc.MultisampleEnable = false;
		rasterizer_desc.ScissorEnable = false;
		rasterizer_desc.SlopeScaledDepthBias = 0.0f;
		ID3D11RasterizerState* rasterizer;
		hr = device_->CreateRasterizerState(&rasterizer_desc, &rasterizer);
		assert(SUCCEEDED(hr));

		D3D11_BLEND_DESC blend_desc;
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = false;
		blend_desc.RenderTarget[0].BlendEnable = false;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		ID3D11BlendState* blend;
		hr = device_->CreateBlendState(&blend_desc, &blend);
		assert(SUCCEEDED(hr));
		
		return make_shared<WindowsPipelineState>(rasterizer, depth_stencil, blend);
	}

	void WindowsDevice::SetPrimitiveTopology(PrimitiveTopology topology)
	{
		device_context_->IASetPrimitiveTopology(gTopologyMap[topology]);
	}

	void WindowsDevice::SetRenderTarget(shared_ptr<IRenderTarget> render_target)
	{
		shared_ptr<WindowsRenderTarget> windows_render_target = static_pointer_cast<WindowsRenderTarget>(render_target);
		current_render_target_ = windows_render_target;

		// Only support one render target now
		ID3D11RenderTargetView* render_target_views[] = { windows_render_target->GetRenderTargetView() };
		device_context_->OMSetRenderTargets(1, render_target_views, windows_render_target->GetDepthStencilView());

		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		vp.Width = static_cast<float>(windows_render_target->GetWidth());
		vp.Height = static_cast<float>(windows_render_target->GetHeight());
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		device_context_->RSSetViewports(1, &vp);
	}

	void WindowsDevice::SetInputLayout(shared_ptr<IInputLayout> input_layout)
	{
		shared_ptr<WindowsInputLayout> layout = static_pointer_cast<WindowsInputLayout>(input_layout);
		device_context_->IASetInputLayout(layout->GetInputLayout());
	}

	void WindowsDevice::SetVertexShader(shared_ptr<IShader> vertex_shader)
	{
		shared_ptr<WindowsShader<ID3D11VertexShader>> shader = static_pointer_cast<WindowsShader<ID3D11VertexShader>>(vertex_shader);
		device_context_->VSSetShader(shader->GetShader(), nullptr, 0);
	}
	void WindowsDevice::SetPixelShader(shared_ptr<IShader> pixel_shader)
	{
		shared_ptr<WindowsShader<ID3D11PixelShader>> shader = static_pointer_cast<WindowsShader<ID3D11PixelShader>>(pixel_shader);
		device_context_->PSSetShader(shader->GetShader(), nullptr, 0);
	}

	void WindowsDevice::SetVertexBuffer(const vector<shared_ptr<IVertexBuffer>>& vertex_buffers, const vector<int> vertex_strides)
	{
		int buffer_count = vertex_buffers.size();
		vector<ID3D11Buffer*> buffers(buffer_count);
		vector<UINT> strides(buffer_count);
		vector<UINT> offsets(buffer_count);

		for (int i = 0; i < buffer_count; ++i)
		{
			shared_ptr<WindowsVertexBuffer> windows_vertex_buffer = static_pointer_cast<WindowsVertexBuffer>(vertex_buffers[i]);
			buffers[i] = windows_vertex_buffer->GetBuffer();
			strides[i] = vertex_strides[i];
			offsets[i] = 0;
		}

		device_context_->IASetVertexBuffers(0, buffer_count, buffers.data(), strides.data(), offsets.data());
	}

	void WindowsDevice::SetPipelineState(shared_ptr<IPipelineState> pipeline_state)
	{
		shared_ptr<WindowsPipelineState> state = static_pointer_cast<WindowsPipelineState>(pipeline_state);
		device_context_->RSSetState(state->GetRasterizerState());
		device_context_->OMSetDepthStencilState(state->GetDepthStencilState(), 1);
		FLOAT blend_factor[] = { 0, 0, 0, 0 };
		device_context_->OMSetBlendState(state->GetBlendState(), blend_factor, 0xffffffff);
	}

	void WindowsDevice::Clear(float r, float g, float b, float a)
	{
		float clear_color[] = { r, g, b, a };
		device_context_->ClearRenderTargetView(current_render_target_->GetRenderTargetView(), clear_color);
		device_context_->ClearDepthStencilView(current_render_target_->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}

	void WindowsDevice::Draw(int vertex_count, int start_vertex_location)
	{
		device_context_->Draw(vertex_count, start_vertex_location);
	}

	void WindowsDevice::Present()
	{
		swap_chain_->Present(1, 0);
	}
}