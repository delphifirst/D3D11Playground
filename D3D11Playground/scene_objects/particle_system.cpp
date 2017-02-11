#include "particle_system.h"

using namespace std;
using namespace DirectX;

ParticleSystem::ParticleSystem(const wstring& name, const wstring& texture_filename,
	FXMVECTOR spawn_pos, float min_speed, float max_speed,
	float min_life_time, float max_life_time, int particle_num)
	: SceneObject(name)
{
	
}