#pragma once

#include <string>
#include <vector>

#include <d3d11.h>

#include "utils.h"

class Shader
{
public:
	Shader()
	{}
	~Shader();
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	void AddShader(ShaderType shader_type, const std::wstring &filename);
	void AddVertexShader(const std::wstring &filename, D3D11_INPUT_ELEMENT_DESC input_elem_desc[], int input_elem_count);
	void Use() const;

private:
	std::vector<char> ReadFile(const std::wstring& filename);

	ID3D11VertexShader* vertex_shader_ = nullptr;
	ID3D11HullShader* hull_shader_ = nullptr;
	ID3D11DomainShader* domain_shader_ = nullptr;
	ID3D11GeometryShader* geometry_shader_ = nullptr;
	ID3D11PixelShader* pixel_shader_ = nullptr;
	ID3D11ComputeShader* compute_shader_ = nullptr;
	ID3D11InputLayout* input_layout_ = nullptr;
};