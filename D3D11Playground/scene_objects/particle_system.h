#pragma once

#include <string>
#include <random>

#include <d3d11.h>
#include <DirectXMath.h>

#include "../engine/scene_object.h"

class ParticleSystem : public SceneObject
{
public:
	struct MatrixBuffer
	{
		DirectX::XMFLOAT4X4 world_matrix;
		DirectX::XMFLOAT4X4 view_matrix;
		DirectX::XMFLOAT4X4 proj_matrix;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 tex_coord;
	};

	struct Particle
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 speed_direction;
		float speed;
		float life_time;
		float current_life_time;
	};

	ParticleSystem(const std::wstring& name, const std::wstring& texture_filename,
		DirectX::FXMVECTOR spawn_pos, float min_speed, float max_speed,
		float min_life_time, float max_life_time, int particle_num);

private:
	float RandomInRange(float min_val, float max_val);
	DirectX::XMVECTOR XM_CALLCONV RandomDirection();
	std::default_random_engine random_engine;
	std::uniform_real_distribution<float> random;

	std::vector<Particle> particles_;
};