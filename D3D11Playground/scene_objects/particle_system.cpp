#include "particle_system.h"

#include <vector>
#include <cmath>

#include "../engine/camera_object.h"

using namespace std;
using namespace DirectX;

ParticleSystem::ParticleSystem(const wstring& name, const wstring& texture_filename,
	float min_speed, float max_speed, float min_life_time, float max_life_time, int particle_num)
	: SceneObject(name), min_speed_(min_speed), max_speed_(max_speed), min_life_time_(min_life_time), max_life_time_(max_life_time)
{
	for (int i = 0; i < particle_num; ++i)
	{
		Particle new_particle;
		new_particle.position = position;
		XMStoreFloat3(&new_particle.speed, RandomDirection() * RandomInRange(min_speed, max_speed));
		new_particle.life_time = RandomInRange(min_life_time, max_life_time);
		new_particle.current_life_time = 0;
		particles_.push_back(new_particle);
	}

	resource_.AddVertexBuffer(nullptr, sizeof(Vertex), sizeof(Vertex) * particle_num * 3, true);
	resource_.AddCBuffer(ShaderType::VS, sizeof(MatrixBuffer));
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

	shader_.AddVertexShader(L"resources/particle_system_vs.cso", input_layout_desc,
		sizeof(input_layout_desc) / sizeof(input_layout_desc[0]));
	shader_.AddShader(ShaderType::PS, L"resources/particle_system_ps.cso");

	render_state_.ChangeBlendState().RenderTarget[0].BlendEnable = true;
	render_state_.ChangeBlendState().RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	render_state_.ChangeBlendState().RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	render_state_.ChangeBlendState().RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	render_state_.ChangeDepthStencilState().DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
}

void ParticleSystem::OnUpdate(double delta_time)
{
	SceneObject::OnUpdate(delta_time);

	for (Particle &p : particles_)
	{
		if (p.current_life_time < p.life_time)
		{
			p.current_life_time += delta_time;
			XMVECTOR p_position = XMLoadFloat3(&p.position);
			XMVECTOR p_speed = XMLoadFloat3(&p.speed);
			p_position += p_speed * delta_time;
			XMStoreFloat3(&p.position, p_position);
		}
		else
		{
			p.position = position;
			XMStoreFloat3(&p.speed, RandomDirection() * RandomInRange(min_speed_, max_speed_));
			p.life_time = RandomInRange(min_life_time_, max_life_time_);
			p.current_life_time = 0;
		}
	}
}

void ParticleSystem::OnDraw()
{
	SceneObject::OnDraw();

	CameraObject* main_camera = Engine::Instance().main_camera();
	XMMATRIX rotation_y_matrix = XMMatrixRotationY(main_camera->rotation.y);
	XMMATRIX rotation_x_matrix = XMMatrixRotationX(main_camera->rotation.x);
	XMMATRIX rotation_z_matrix = XMMatrixRotationZ(main_camera->rotation.z);
	XMMATRIX billboard_rotation = rotation_z_matrix * rotation_x_matrix * rotation_y_matrix;

	Vertex *vertices = resource_.MapVertexBuffer<Vertex>(0);
	for (const Particle& p : particles_)
	{
		const float kParticleSize = 1;
		XMVECTOR v0 = XMVectorSet(-kParticleSize / 2, kParticleSize / 2, 0, 1);
		XMVECTOR v1 = XMVectorSet(3 * kParticleSize / 2, kParticleSize / 2, 0, 1);
		XMVECTOR v2 = XMVectorSet(-kParticleSize / 2, -3 * kParticleSize / 2, 0, 1);
		v0 = XMVector3TransformCoord(v0, billboard_rotation);
		v1 = XMVector3TransformCoord(v1, billboard_rotation);
		v2 = XMVector3TransformCoord(v2, billboard_rotation);

		XMVECTOR p_position = XMLoadFloat3(&p.position);
		v0 += p_position;
		v1 += p_position;
		v2 += p_position;
		
		XMStoreFloat3(&vertices->position, v0);
		vertices->tex_coord = XMFLOAT2(0, 0);
		++vertices;
		XMStoreFloat3(&vertices->position, v1);
		vertices->tex_coord = XMFLOAT2(2, 0);
		++vertices;
		XMStoreFloat3(&vertices->position, v2);
		vertices->tex_coord = XMFLOAT2(0, 2);
		++vertices;
	}
	resource_.UnmapVertexBuffer(0);

	MatrixBuffer matrix_buffer;
	XMMATRIX transposed_matrix;

	transposed_matrix = Engine::Instance().main_camera()->GetViewMatrix();
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.view_matrix, transposed_matrix);

	transposed_matrix = Engine::Instance().main_camera()->GetProjectMatrix(
		Engine::Instance().width(), Engine::Instance().height());
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.proj_matrix, transposed_matrix);

	resource_.UpdateCBuffer(ShaderType::VS, 0, &matrix_buffer, sizeof(matrix_buffer));

	Engine::Instance().device_context()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::Instance().device_context()->Draw(particles_.size() * 3, 0);
}

float ParticleSystem::RandomInRange(float min_val, float max_val)
{
	return min_val + (max_val - min_val) * random(random_engine);
}

XMVECTOR XM_CALLCONV ParticleSystem::RandomDirection()
{
	float theta = RandomInRange(0, XM_2PI);
	float z = RandomInRange(-1, 1);
	float r = sqrt(1 - z * z);
	return XMVectorSet(r * cos(theta), r * sin(theta), z, 0);
}