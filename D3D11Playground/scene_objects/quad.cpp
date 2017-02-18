#include "quad.h"

#include <cassert>

#include "../engine/engine.h"

using namespace std;
using namespace DirectX;

Quad::Quad(const wstring& name, FXMVECTOR a, FXMVECTOR b,
	FXMVECTOR c, CXMVECTOR d, const wstring &texture_filename)
	: Object(name), 
	vertex_shader_(L"resources/quad_vs.cso"), hull_shader_(L"resources/quad_hs.cso"),
	domain_shader_(L"resources/quad_ds.cso"), pixel_shader_(L"resources/quad_ps.cso")
{
	Vertex vertices[4];
	XMStoreFloat3(&vertices[0].position, a);
	vertices[0].tex_coord = XMFLOAT2(0, 0);
	XMStoreFloat3(&vertices[1].position, b);
	vertices[1].tex_coord = XMFLOAT2(1, 0);
	XMStoreFloat3(&vertices[2].position, c);
	vertices[2].tex_coord = XMFLOAT2(0, 1);
	XMStoreFloat3(&vertices[3].position, d);
	vertices[3].tex_coord = XMFLOAT2(1, 1);
	resource_.AddVertexBuffer(vertices, sizeof(Vertex), sizeof(vertices), false);

	resource_.AddCBuffer(ShaderType::DS, sizeof(MatrixBuffer));
	resource_.AddTexture(ShaderType::PS, texture_filename);

	D3D11_INPUT_ELEMENT_DESC input_layout_desc[2];

	input_layout_desc[0].SemanticName = "POSITION";
	input_layout_desc[0].SemanticIndex = 0;
	input_layout_desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	input_layout_desc[0].InputSlot = 0;
	input_layout_desc[0].AlignedByteOffset = 0;
	input_layout_desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[0].InstanceDataStepRate = 0;

	input_layout_desc[1].SemanticName = "TEXCOORD";
	input_layout_desc[1].SemanticIndex = 0;
	input_layout_desc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	input_layout_desc[1].InputSlot = 0;
	input_layout_desc[1].AlignedByteOffset = 12;
	input_layout_desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[1].InstanceDataStepRate = 0;

	vertex_shader_.UpdateInputLayout(input_layout_desc,
		sizeof(input_layout_desc) / sizeof(input_layout_desc[0]));
}

void Quad::OnDraw()
{
	resource_.Use();
	render_state_.Use();
	ClearAllShaders();
	vertex_shader_.Use();
	hull_shader_.Use();
	domain_shader_.Use();
	pixel_shader_.Use();

	resource_.IASetVertexBuffers({ 0 });

	MatrixBuffer matrix_buffer;
	XMMATRIX transposed_matrix;

	transposed_matrix = XMLoadFloat4x4(&world_transform());
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.world_matrix, transposed_matrix);

	transposed_matrix = Engine::Instance().main_camera()->GetViewMatrix();
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.view_matrix, transposed_matrix);

	transposed_matrix = Engine::Instance().main_camera()->GetProjectMatrix(
		Engine::Instance().width(), Engine::Instance().height());
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.proj_matrix, transposed_matrix);

	resource_.UpdateCBuffer(ShaderType::DS, 0, &matrix_buffer, sizeof(matrix_buffer));

	Engine::Instance().device_context()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	Engine::Instance().device_context()->Draw(4, 0);
}
