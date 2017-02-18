#pragma once

#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

#include "../engine/scene_object.h"

class Quad : public SceneObject
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

	Quad(const std::wstring& name, DirectX::FXMVECTOR a, DirectX::FXMVECTOR b,
		DirectX::FXMVECTOR c, DirectX::CXMVECTOR d, const std::wstring &texture_filename);

	virtual void OnDraw() override;

};