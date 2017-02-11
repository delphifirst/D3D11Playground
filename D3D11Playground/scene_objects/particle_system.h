#pragma once

#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

#include "../engine/scene_object.h"

class ParticleSystem : public SceneObject
{
public:
	ParticleSystem(const std::wstring& name, const std::wstring& texture_filename,
		DirectX::FXMVECTOR spawn_pos, float min_speed, float max_speed,
		float min_life_time, float max_life_time, int particle_num);
};