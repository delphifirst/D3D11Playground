#include "particle_system.h"

#include <vector>
#include <cmath>

using namespace std;
using namespace DirectX;

ParticleSystem::ParticleSystem(const wstring& name, const wstring& texture_filename,
	FXMVECTOR spawn_pos, float min_speed, float max_speed,
	float min_life_time, float max_life_time, int particle_num)
	: SceneObject(name)
{
	for (int i = 0; i < particle_num; ++i)
	{
		Particle new_particle;
		XMStoreFloat3(&new_particle.position, spawn_pos);
		XMStoreFloat3(&new_particle.speed_direction, RandomDirection());
		new_particle.speed = RandomInRange(min_speed, max_speed);
		new_particle.life_time = RandomInRange(min_life_time, max_life_time);
		new_particle.current_life_time = 0;
		particles_.push_back(new_particle);
	}
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