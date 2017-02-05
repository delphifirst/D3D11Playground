#include "quad.h"

#include "../engine/engine.h"

using namespace std;
using namespace DirectX;

Quad::Quad(const wstring& name, void *vertex_data, int bytes,
	const wstring &texture_filename)
	: SceneObject(name, vertex_data, sizeof(Vertex), bytes)
{
	resource_.AddCBuffer(ShaderType::VS, sizeof(MatrixBuffer));
	resource_.AddTexture(ShaderType::PS, texture_filename);

	shader_.AddShader(ShaderType::VS, L"resources/default_vs.cso");
	shader_.AddShader(ShaderType::PS, L"resources/default_ps.cso");
}

void Quad::OnUpdate(double delta_time)
{
	SceneObject::OnUpdate(delta_time);
}

void Quad::OnDraw()
{
	SceneObject::OnDraw();

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

	resource_.UpdateCBuffer(0, &matrix_buffer, sizeof(matrix_buffer));

	Engine::Instance().device_context()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Engine::Instance().device_context()->Draw(4, 0);
}

Quad* XM_CALLCONV MakeQuad(const wstring& name, FXMVECTOR a, FXMVECTOR b,
	FXMVECTOR c, GXMVECTOR d, const wstring& texture_filename)
{
	Quad::Vertex vertices[4];
	XMStoreFloat3(&vertices[0].position, a);
	vertices[0].tex_coord = XMFLOAT2(0, 0);
	XMStoreFloat3(&vertices[1].position, b);
	vertices[1].tex_coord = XMFLOAT2(1, 0);
	XMStoreFloat3(&vertices[2].position, c);
	vertices[2].tex_coord = XMFLOAT2(0, 1);
	XMStoreFloat3(&vertices[3].position, d);
	vertices[3].tex_coord = XMFLOAT2(1, 1);
	return new Quad(name, vertices, sizeof(vertices), texture_filename);
}