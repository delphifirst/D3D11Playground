#pragma once

#include <memory>
#include <array>

#include "render_object.h"
#include "color.h"
#include "vec.h"
#include "mat.h"

namespace playground
{
	class IVertexBuffer;
	class IIndexBuffer;
	class IInputLayout;
	class IShader;
	class IPipelineState;
	class IDevice;
	class MainRenderer;
	class Camera;

	class ColorCube : public IRenderObject
	{
	public:
		ColorCube(const Vec3f& color);
		virtual void PreDraw(MainRenderer* main_renderer) override;
		virtual void Draw(MainRenderer* main_renderer, Camera* camera) override;

		const Vec3f GetPosition() const
		{
			return position_;
		}
		const Vec3f GetScale() const
		{
			return scale_;
		}
		const Vec3f GetRotation() const
		{
			return rotation_;
		}

		void SetPosition(const Vec3f& position)
		{
			position_ = position;
			dirty_ = true;
		}
		void SetScale(const Vec3f& scale)
		{
			scale_ = scale;
			dirty_ = true;
		}
		void SetRotation(const Vec3f& rotation)
		{
			rotation_ = rotation;
			dirty_ = true;
		}

	private:
		struct VertexType
		{
			Vec3f position;
		};
		std::array<VertexType, 8> vertices_;
		std::array<uint16_t, 36> indices_;
		Vec3f color_;
		bool initialized_ = false;

		Vec3f position_ = Vec3f(0, 0, 0);
		Vec3f scale_ = Vec3f(1, 1, 1);
		Vec3f rotation_ = Vec3f(0, 0, 0);
		Mat4x4f world_transform_;
		bool dirty_ = true;

		std::shared_ptr<IVertexBuffer> vertex_buffer_;
		std::shared_ptr<IIndexBuffer> index_buffer_;
		std::shared_ptr<IInputLayout> input_layout_;
		std::shared_ptr<IShader> vertex_shader_;
		std::shared_ptr<IShader> pixel_shader_;
		std::shared_ptr<IPipelineState> pipeline_state_;
	};
}