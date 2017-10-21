#include "color_cube.h"

#include <vector>

#include "buffer.h"
#include "main_renderer.h"
#include "device.h"
#include "utils.h"
#include "input_layout.h"
#include "platform_constant.h"
#include "shader.h"
#include "camera.h"

using namespace std;

namespace playground
{
	ColorCube::ColorCube(const Vec3f& color)
		:color_(color)
	{
		vertices_[0].position = Vec3f(1, 1, 1);
		vertices_[1].position = Vec3f(1, 1, -1);
		vertices_[2].position = Vec3f(-1, 1, -1);
		vertices_[3].position = Vec3f(-1, 1, 1);
		vertices_[4].position = Vec3f(1, -1, 1);
		vertices_[5].position = Vec3f(1, -1, -1);
		vertices_[6].position = Vec3f(-1, -1, -1);
		vertices_[7].position = Vec3f(-1, -1, 1);

		uint16_t indices[] = {0, 1, 2, 0, 2, 3, 4, 6, 5, 4, 7, 6, 3, 2, 6, 3, 6, 7, 0, 4, 5, 0, 5, 1, 0, 3, 7, 0, 7, 4, 1, 5, 6, 1, 6, 2};
		for (int i = 0; i < sizeof(indices) / sizeof(indices[0]); ++i)
		{
			indices_[i] = indices[i];
		}
	}

	void ColorCube::PreDraw(MainRenderer* main_renderer)
	{
		if (!initialized_)
		{
			initialized_ = true;
			vertex_buffer_ = main_renderer->GetDevice()->CreateVertexBuffer(vertices_.data(), vertices_.size() * sizeof(VertexType));
			index_buffer_ = main_renderer->GetDevice()->CreateIndexBuffer(indices_.data(), indices_.size() * sizeof(uint16_t), INDEX_FORMAT_UINT16);
			vertex_shader_ = main_renderer->GetDevice()->CreateVertexShader("color_vs", ReadFile("resources/color_vs.cso"));
			pixel_shader_ = main_renderer->GetDevice()->CreatePixelShader("color_ps", ReadFile("resources/color_ps.cso"));
			vector<InputElementDesc> desc = {
				InputElementDesc{ "POSITION", VERTEX_FORMAT_R32G32B32_FLOAT, 0, 0 }
			};
			input_layout_ = main_renderer->GetDevice()->CreateInputLayout(desc, vertex_shader_);
			pipeline_state_ = main_renderer->GetDevice()->CreatePipelineState();
		}

		if (dirty_)
		{
			dirty_ = false;
			world_transform_ = Mat4x4f::Translation(position_) 
				* Mat4x4f::RotationY(rotation_.y) * Mat4x4f::RotationX(rotation_.x) * Mat4x4f::RotationZ(rotation_.z) 
				* Mat4x4f::Scale(scale_);
		}
	}

	void ColorCube::Draw(MainRenderer* main_renderer, Camera* camera)
	{
		vertex_shader_->SetVariable("view_proj", camera->GetViewProjMat().GetData());
		vertex_shader_->SetVariable("world", world_transform_.GetData());
		pixel_shader_->SetVariable("color", &Vec4f(color_, 1));
		main_renderer->GetDevice()->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		main_renderer->GetDevice()->SetInputLayout(input_layout_);
		main_renderer->GetDevice()->SetVertexBuffer({ vertex_buffer_ }, { sizeof(VertexType) });
		main_renderer->GetDevice()->SetIndexBuffer(index_buffer_);
		main_renderer->GetDevice()->SetPipelineState(pipeline_state_);
		main_renderer->GetDevice()->SetVertexShader(vertex_shader_);
		main_renderer->GetDevice()->SetPixelShader(pixel_shader_);
		main_renderer->GetDevice()->DrawIndexed(36, 0, 0);
	}
}