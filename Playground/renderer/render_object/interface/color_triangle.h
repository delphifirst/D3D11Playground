#pragma once

#include <memory>
#include <array>

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
	class Camera;

	class ColorTriangle : public IRenderObject
	{
	public:
		ColorTriangle(const Vec3f& p0, const Vec3f& c0, const Vec3f& p1, const Vec3f& c1, const Vec3f& p2, const Vec3f& c2);
		virtual void PreDraw(MainRenderer* main_renderer) override;
		virtual void Draw(MainRenderer* main_renderer, Camera* camera) override;
	private:
		struct VertexType
		{
			Vec3f position;
			Vec3f color;
		};
		std::array<VertexType, 3> vertices_;
		bool dirty_ = true;

		std::shared_ptr<IVertexBuffer> vertex_buffer_;
		std::shared_ptr<IInputLayout> input_layout_;
		std::shared_ptr<IShader> vertex_shader_;
		std::shared_ptr<IShader> pixel_shader_;
		std::shared_ptr<IPipelineState> pipeline_state_;
	};
}