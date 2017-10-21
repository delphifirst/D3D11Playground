#include "color_triangle.h"

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
	ColorTriangle::ColorTriangle(const Vec3f& p0, const Vec3f& c0, const Vec3f& p1, const Vec3f& c1, const Vec3f& p2, const Vec3f& c2)
	{
		vertices_[0].position = p0;
		vertices_[0].color = c0;
		vertices_[1].position = p1;
		vertices_[1].color = c1;
		vertices_[2].position = p2;
		vertices_[2].color = c2;
	}

	void ColorTriangle::PreDraw(MainRenderer* main_renderer)
	{
		if (dirty_)
		{
			dirty_ = false;
			vertex_buffer_ = main_renderer->GetDevice()->CreateVertexBuffer(vertices_.data(), vertices_.size() * sizeof(VertexType));
			vertex_shader_ = main_renderer->GetDevice()->CreateVertexShader("test_vs", ReadFile("resources/test_vs.cso"));
			pixel_shader_ = main_renderer->GetDevice()->CreatePixelShader("test_ps", ReadFile("resources/test_ps.cso"));
			vector<InputElementDesc> desc = {
				InputElementDesc{ "POSITION", VERTEX_FORMAT_R32G32B32_FLOAT, 0, 0 },
				InputElementDesc{ "COLOR", VERTEX_FORMAT_R32G32B32_FLOAT, 0, 12 }
			};
			input_layout_ = main_renderer->GetDevice()->CreateInputLayout(desc, vertex_shader_);
			pipeline_state_ = main_renderer->GetDevice()->CreatePipelineState();
		}
	}

	void ColorTriangle::Draw(MainRenderer* main_renderer, Camera* camera)
	{
		vertex_shader_->SetVariable("view_proj", camera->GetViewProjMat().GetData());
		main_renderer->GetDevice()->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		main_renderer->GetDevice()->SetInputLayout(input_layout_);
		main_renderer->GetDevice()->SetVertexBuffer({ vertex_buffer_ }, { sizeof(VertexType) });
		main_renderer->GetDevice()->SetPipelineState(pipeline_state_);
		main_renderer->GetDevice()->SetVertexShader(vertex_shader_);
		main_renderer->GetDevice()->SetPixelShader(pixel_shader_);
		main_renderer->GetDevice()->Draw(3, 0);
	}
}