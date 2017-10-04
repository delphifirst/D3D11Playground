#pragma once

#include <string>
#include <random>

#include <d3d11.h>
#include <DirectXMath.h>

#include "../engine/object.h"
#include "../engine/shader.h"
#include "../engine/render_state.h"
#include "../engine/resource.h"

class ParticleSystem : public Object
{
public:
	struct MatrixBuffer
	{
		DirectX::XMFLOAT4 x;
		DirectX::XMFLOAT4 y;
		DirectX::XMFLOAT4 size;
		DirectX::XMFLOAT4X4 view_matrix;
		DirectX::XMFLOAT4X4 proj_matrix;
	};

	struct SOBuffer
	{
		DirectX::XMFLOAT3 emit_position;
		float min_speed;
		float max_speed;
		float min_life_time;
		float max_life_time;
		float delta_time;
	};

	struct Particle
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 speed;
		float life_time;
		float current_life_time;
	};

	ParticleSystem(const std::wstring& name, const std::wstring& texture_filename,
		float min_speed, float max_speed, float min_life_time, float max_life_time, 
		int particle_num);

	virtual void OnUpdate(double delta_time) override;
	virtual void OnDraw() override;

private:
	VertexShader vertex_shader_;
	GeometryShader geometry_shader_;
	GeometrySOShader geometry_so_shader_;
	PixelShader pixel_shader_;
	RenderState render_state_;
	Resource resource_;

	float RandomInRange(float min_val, float max_val);
	DirectX::XMVECTOR XM_CALLCONV RandomDirection();
	std::default_random_engine random_engine;
	std::uniform_real_distribution<float> random;

	double last_delta_time_ = 0;
	int current_vertex_buffer_index_ = 0;

	float min_speed_, max_speed_;
	float min_life_time_, max_life_time_;
	int particle_num_;
};