#pragma once

#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

#include "../engine/scene_object.h"

class Quad : public SceneObject
{
	friend Quad* XM_CALLCONV MakeQuad(const std::wstring& name, DirectX::FXMVECTOR a, 
		DirectX::FXMVECTOR b, DirectX::FXMVECTOR c, DirectX::GXMVECTOR, 
		const std::wstring& texture_filename);

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

	struct CSBuffer
	{
		int size;
		int unused[3];
	};

	virtual void OnUpdate(double delta_time) override;
	virtual void OnDraw() override;

private:
	Quad(const std::wstring& name, void *vertex_data, int bytes, 
		const std::wstring &texture_filename);

	// Stuff for testing compute shader
	void InitComputeShaderTest();
	void ComputeShaderTest();
	static constexpr int kMatrixSize = 32;
	float input_data[kMatrixSize * kMatrixSize];
	float cpu_result[kMatrixSize * kMatrixSize];
	float gpu_result[kMatrixSize * kMatrixSize];
};

Quad* XM_CALLCONV MakeQuad(const std::wstring& name, DirectX::FXMVECTOR a, DirectX::FXMVECTOR b,
	DirectX::FXMVECTOR c, DirectX::GXMVECTOR d, const std::wstring& texture_filename);