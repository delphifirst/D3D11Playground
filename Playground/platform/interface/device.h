#pragma once

#include <memory>
#include <vector>

#include "platform_constant.h"

namespace playground
{
	class IDisplayer;
	class IRenderTarget;
	class IVertexBuffer;
	class IInputLayout;
	struct InputElementDesc;
	class IShader;
	class IPipelineState;

	class IDevice
	{
	public:
		virtual void Init() = 0;
		virtual void Fini() = 0;

		virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(void* init_data, int length) = 0;
		virtual std::shared_ptr<IInputLayout> CreateInputLayout(
			const std::vector<InputElementDesc>& input_desc, std::shared_ptr<IShader> vertex_shader) = 0;
		virtual std::shared_ptr<IShader> CreateVertexShader(const std::string& name, const std::vector<char>& code) = 0;
		virtual std::shared_ptr<IShader> CreatePixelShader(const std::string& name, const std::vector<char>& code) = 0;
		virtual std::shared_ptr<IPipelineState> CreatePipelineState() = 0;

		virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
		virtual void SetRenderTarget(std::shared_ptr<IRenderTarget> render_target) = 0;
		virtual void SetInputLayout(std::shared_ptr<IInputLayout> input_layout) = 0;
		virtual void SetVertexShader(std::shared_ptr<IShader> vertex_shader) = 0;
		virtual void SetPixelShader(std::shared_ptr<IShader> pixel_shader) = 0;
		virtual void SetVertexBuffer(const std::vector<std::shared_ptr<IVertexBuffer>>& vertex_buffers, const std::vector<int> strides) = 0;
		virtual void SetPipelineState(std::shared_ptr<IPipelineState> pipeline_state) = 0;
		
		virtual void Clear(float r, float g, float b, float a) = 0;
		virtual void Draw(int vertex_count, int start_vertex_location) = 0;
		virtual void Present() = 0;
	};

	void CreateDevice();
	extern std::shared_ptr<IDevice> gDevice;
}