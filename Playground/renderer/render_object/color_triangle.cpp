#include "color_triangle.h"

#include <array>
#include <fstream>

#include "buffer.h"
#include "main_renderer.h"
#include "device.h"
#include "utils.h"
#include "input_layout.h"
#include "constant.h"

using namespace std;

namespace playground
{
	ColorTriangle::ColorTriangle(shared_ptr<MainRenderer> main_renderer, 
		const Vec3f& p0, const Vec3f& c0, const Vec3f& p1, const Vec3f& c1, const Vec3f& p2, const Vec3f& c2)
		: RenderObject(main_renderer)
	{
		array<VertexType, 3> vertices;

		vertices[0].position = p0;
		vertices[0].color = c0;
		vertices[1].position = p1;
		vertices[1].color = c1;
		vertices[2].position = p2;
		vertices[2].color = c2;

		vertex_buffer_ = main_renderer->GetDevice()->CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(VertexType));
		vertex_shader_ = main_renderer->GetDevice()->CreateVertexShader("test_vs", ReadFile("resources/test_vs.cso"));
		pixel_shader_ = main_renderer->GetDevice()->CreatePixelShader("test_ps", ReadFile("resources/test_ps.cso"));
		vector<InputElementDesc> desc = {
			InputElementDesc{"POSITION", VERTEX_FORMAT_R32G32B32_FLOAT, 0, 0},
			InputElementDesc{"COLOR", VERTEX_FORMAT_R32G32B32_FLOAT, 0, 12}
		};
		input_layout_ = main_renderer->GetDevice()->CreateInputLayout(desc, vertex_shader_);
		pipeline_state_ = main_renderer->GetDevice()->CreatePipelineState();
	}
	void ColorTriangle::Draw()
	{
		main_renderer_->GetDevice()->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		main_renderer_->GetDevice()->SetInputLayout(input_layout_);
		main_renderer_->GetDevice()->SetVertexBuffer({ vertex_buffer_ }, { sizeof(VertexType) });
		main_renderer_->GetDevice()->SetVertexShader(vertex_shader_);
		main_renderer_->GetDevice()->SetPixelShader(pixel_shader_);
		main_renderer_->GetDevice()->SetPipelineState(pipeline_state_);
		main_renderer_->GetDevice()->Draw(3, 0);
	}
}