#include "windows_constant.h"

#include <d3d11.h>

namespace playground
{
	uint32_t gVertexFormatMap[VERTEX_FORMAT_SIZE];
	D3D11_PRIMITIVE_TOPOLOGY gTopologyMap[PRIMITIVE_TOPOLOGY_SIZE];

	void SetupConstantMap()
	{
		gVertexFormatMap[VERTEX_FORMAT_R32G32B32_FLOAT] = DXGI_FORMAT_R32G32B32_FLOAT;

		gTopologyMap[PRIMITIVE_TOPOLOGY_TRIANGLELIST] = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
}