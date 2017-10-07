#pragma once

#include <cstdint>

#include <d3d11.h>

#include "platform_constant.h"

namespace playground
{
	extern uint32_t gFormatMap[VERTEX_FORMAT_SIZE];
	extern D3D11_PRIMITIVE_TOPOLOGY gTopologyMap[PRIMITIVE_TOPOLOGY_SIZE];

	void SetupConstantMap();
}