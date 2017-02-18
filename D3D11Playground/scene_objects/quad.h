#pragma once

#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

#include "../engine/object.h"
#include "../engine/shader.h"
#include "../engine/render_state.h"
#include "../engine/resource.h"

class Quad : public Object
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
private:
	Shader shader_;
	RenderState render_state_;
	Resource resource_;
};