#pragma once

#include <memory>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "device.h"

#include "windows_render_target.h"

namespace playground
{
	template<typename T>
	class WindowsShader;

	class WindowsDevice : public IDevice
	{
	public:
		virtual void Init() override;
		virtual void Fini() override;

		virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(void* init_data, int length) override;
		virtual std::shared_ptr<IInputLayout> CreateInputLayout(
			const std::vector<InputElementDesc>& input_desc, std::shared_ptr<IShader> vertex_shader) override;
		virtual std::shared_ptr<IShader> CreateVertexShader(const std::string& name, const std::vector<char>& code) override;
		virtual std::shared_ptr<IShader> CreatePixelShader(const std::string& name, const std::vector<char>& code) override;
		virtual std::shared_ptr<IPipelineState> CreatePipelineState() override;

		virtual void SetPrimitiveTopology(PrimitiveTopology topology) override;
		virtual void SetRenderTarget(std::shared_ptr<IRenderTarget> render_target) override;
		virtual void SetInputLayout(std::shared_ptr<IInputLayout> input_layout) override;
		virtual void SetVertexShader(std::shared_ptr<IShader> vertex_shader) override;
		virtual void SetPixelShader(std::shared_ptr<IShader> pixel_shader) override;
		virtual void SetVertexBuffer(const std::vector<std::shared_ptr<IVertexBuffer>>& vertex_buffers, const std::vector<int> vertex_strides) override;
		virtual void SetPipelineState(std::shared_ptr<IPipelineState> pipeline_state) override;

		virtual void Clear(float r, float g, float b, float a) override;
		virtual void Draw(int vertex_count, int start_vertex_location) override;
		virtual void Present() override;
	private:
		void CreateDefaultRenderTarget(int width, int height);
		void PrepareShaderReflection(const std::vector<char>& code, 
			ID3D11ShaderReflection** shader_reflection, int* constant_buffer_size);
		void PrepareConstantBuffer(int length, ID3D11Buffer** constant_buffer);
		template<typename T>
		void UpdateConstantBuffer(std::shared_ptr<WindowsShader<T>> shader);
		std::shared_ptr<WindowsRenderTarget> default_render_target_;
		std::shared_ptr<WindowsRenderTarget> current_render_target_;

		ID3D11Device* device_ = nullptr;
		ID3D11DeviceContext* device_context_ = nullptr;
		IDXGISwapChain* swap_chain_ = nullptr;
	};

	extern std::shared_ptr<WindowsDevice> gWindowsDevice;
}