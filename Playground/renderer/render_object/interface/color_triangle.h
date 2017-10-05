#pragma once

#include <memory>

#include "render_object.h"
#include "color.h"
#include "vec.h"


namespace playground
{
	class IVertexBuffer;
	class IInputLayout;
	class IShader;
	class IPipelineState;
	class IDevice;
	class MainRenderer;

	class ColorTriangle : public IRenderObject
	{
	public:
		ColorTriangle(std::shared_ptr<MainRenderer> main_renderer, 
			const Vec3f& p0, const Vec3f& c0, const Vec3f& p1, const Vec3f& c1, const Vec3f& p2, const Vec3f& c2);
		virtual void Draw(std::shared_ptr<IDevice> device) override;
	private:
		struct VertexType
		{
			Vec3f position;
			Vec3f color;
		};
		std::shared_ptr<IVertexBuffer> vertex_buffer_;
		std::shared_ptr<IInputLayout> input_layout_;
		std::shared_ptr<IShader> vertex_shader_;
		std::shared_ptr<IShader> pixel_shader_;
		std::shared_ptr<IPipelineState> pipeline_state_;
	};
}