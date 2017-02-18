#include "particle_system.h"

#include <vector>
#include <cmath>

#include "../engine/camera_object.h"

using namespace std;
using namespace DirectX;

ParticleSystem::ParticleSystem(const wstring& name, const wstring& texture_filename,
	float min_speed, float max_speed, float min_life_time, float max_life_time, int particle_num)
	: Object(name), vertex_shader_(L"resources/particle_system_vs.cso"),
	geometry_shader_(L"resources/particle_system_gs.cso"), 
	geometry_so_shader_(L"resources/particle_system_gsso.cso"),
	pixel_shader_(L"resources/particle_system_ps.cso"),
	min_speed_(min_speed), max_speed_(max_speed), min_life_time_(min_life_time), 
	max_life_time_(max_life_time),
	particle_num_(particle_num)
{
	vector<Particle> init_particles;
	for (int i = 0; i < particle_num; ++i)
	{
		Particle new_particle;
		new_particle.position = position;
		XMStoreFloat3(&new_particle.speed, RandomDirection() * RandomInRange(min_speed, max_speed));
		new_particle.life_time = RandomInRange(min_life_time, max_life_time);
		new_particle.current_life_time = 0;
		init_particles.push_back(new_particle);
	}

	resource_.AddVertexBuffer(init_particles.data(), sizeof(Particle), sizeof(Particle) * particle_num, false, true);
	resource_.AddVertexBuffer(nullptr, sizeof(Particle), sizeof(Particle) * particle_num, false, true);
	resource_.AddCBuffer(ShaderType::GS, sizeof(MatrixBuffer));
	resource_.AddCBuffer(ShaderType::GS, sizeof(SOBuffer));
	resource_.AddTexture(ShaderType::PS, texture_filename);

	D3D11_INPUT_ELEMENT_DESC input_layout_desc[3];

	input_layout_desc[0].SemanticName = "POSITION";
	input_layout_desc[0].SemanticIndex = 0;
	input_layout_desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	input_layout_desc[0].InputSlot = 0;
	input_layout_desc[0].AlignedByteOffset = 0;
	input_layout_desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[0].InstanceDataStepRate = 0;

	input_layout_desc[1].SemanticName = "SPEED";
	input_layout_desc[1].SemanticIndex = 0;
	input_layout_desc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	input_layout_desc[1].InputSlot = 0;
	input_layout_desc[1].AlignedByteOffset = 12;
	input_layout_desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[1].InstanceDataStepRate = 0;

	input_layout_desc[2].SemanticName = "LIFE_TIME_INFO";
	input_layout_desc[2].SemanticIndex = 0;
	input_layout_desc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	input_layout_desc[2].InputSlot = 0;
	input_layout_desc[2].AlignedByteOffset = 24;
	input_layout_desc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[2].InstanceDataStepRate = 0;

	vertex_shader_.UpdateInputLayout(input_layout_desc,
		sizeof(input_layout_desc) / sizeof(input_layout_desc[0]));

	D3D11_SO_DECLARATION_ENTRY output_layout_desc[3];
	output_layout_desc[0].Stream = 0;
	output_layout_desc[0].SemanticName = "POSITION";
	output_layout_desc[0].SemanticIndex = 0;
	output_layout_desc[0].StartComponent = 0;
	output_layout_desc[0].ComponentCount = 3;
	output_layout_desc[0].OutputSlot = 0;

	output_layout_desc[1].Stream = 0;
	output_layout_desc[1].SemanticName = "SPEED";
	output_layout_desc[1].SemanticIndex = 0;
	output_layout_desc[1].StartComponent = 0;
	output_layout_desc[1].ComponentCount = 3;
	output_layout_desc[1].OutputSlot = 0;

	output_layout_desc[2].Stream = 0;
	output_layout_desc[2].SemanticName = "LIFE_TIME_INFO";
	output_layout_desc[2].SemanticIndex = 0;
	output_layout_desc[2].StartComponent = 0;
	output_layout_desc[2].ComponentCount = 2;
	output_layout_desc[2].OutputSlot = 0;

	geometry_so_shader_.UpdateOutputLayout(output_layout_desc, 
		sizeof(output_layout_desc) / sizeof(output_layout_desc[0]), { sizeof(Particle) });

	render_state_.ChangeBlendState().RenderTarget[0].BlendEnable = true;
	render_state_.ChangeBlendState().RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	render_state_.ChangeBlendState().RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	render_state_.ChangeBlendState().RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	render_state_.ChangeDepthStencilState().DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
}

void ParticleSystem::OnUpdate(double delta_time)
{
	Object::OnUpdate(delta_time);

	last_delta_time_ = delta_time;
}

void ParticleSystem::OnDraw()
{
	resource_.Use();
	render_state_.Use();
	ClearAllShaders();

	vertex_shader_.Use();
	geometry_so_shader_.Use();

	SOBuffer so_buffer;
	so_buffer.emit_position = position;
	so_buffer.min_speed = min_speed_;
	so_buffer.max_speed = max_speed_;
	so_buffer.min_life_time = min_life_time_;
	so_buffer.max_life_time = max_life_time_;
	so_buffer.delta_time = last_delta_time_;
	resource_.UpdateCBuffer(ShaderType::GS, 1, &so_buffer, sizeof(so_buffer));

	resource_.IASetVertexBuffers({ current_vertex_buffer_index_ });
	resource_.SoSetTargets({ 1 - current_vertex_buffer_index_ });
	Engine::Instance().device_context()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Engine::Instance().device_context()->Draw(particle_num_, 0);
	resource_.SoClearTargets();
	current_vertex_buffer_index_ = 1 - current_vertex_buffer_index_;
	
	geometry_shader_.Use();
	pixel_shader_.Use();

	CameraObject* main_camera = Engine::Instance().main_camera();
	XMMATRIX rotation_y_matrix = XMMatrixRotationY(main_camera->rotation.y);
	XMMATRIX rotation_x_matrix = XMMatrixRotationX(main_camera->rotation.x);
	XMMATRIX rotation_z_matrix = XMMatrixRotationZ(main_camera->rotation.z);
	XMMATRIX billboard_rotation = rotation_z_matrix * rotation_x_matrix * rotation_y_matrix;
	XMVECTOR x = billboard_rotation.r[0];
	XMVECTOR y = billboard_rotation.r[1];

	MatrixBuffer matrix_buffer;

	XMStoreFloat4(&matrix_buffer.x, x);
	XMStoreFloat4(&matrix_buffer.y, y);
	const float kParticleSize = 0.1;
	XMStoreFloat4(&matrix_buffer.size, XMVectorSet(kParticleSize, kParticleSize, 0, 0));

	XMMATRIX transposed_matrix;

	transposed_matrix = Engine::Instance().main_camera()->GetViewMatrix();
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.view_matrix, transposed_matrix);

	transposed_matrix = Engine::Instance().main_camera()->GetProjectMatrix(
		Engine::Instance().width(), Engine::Instance().height());
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.proj_matrix, transposed_matrix);

	resource_.UpdateCBuffer(ShaderType::GS, 0, &matrix_buffer, sizeof(matrix_buffer));

	Engine::Instance().device_context()->Draw(particle_num_, 0);
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