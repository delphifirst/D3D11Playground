#include "shader.h"

#include <cassert>
#include <fstream>

#include "utils.h"
#include "engine.h"

using namespace std;


Shader::~Shader()
{
	SafeRelease(vertex_shader_);
	SafeRelease(hull_shader_);
	SafeRelease(domain_shader_);
	SafeRelease(pixel_shader_);
	SafeRelease(compute_shader_);
	SafeRelease(input_layout_);
}

void Shader::InitInputLayout(const vector<char> &vertex_shader_code)
{
	D3D11_INPUT_ELEMENT_DESC input_layout_desc[2];

	input_layout_desc[0].SemanticName = "POSITION";
	input_layout_desc[0].SemanticIndex = 0;
	input_layout_desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	input_layout_desc[0].InputSlot = 0;
	input_layout_desc[0].AlignedByteOffset = 0;
	input_layout_desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[0].InstanceDataStepRate = 0;

	input_layout_desc[1].SemanticName = "TEXCOORD";
	input_layout_desc[1].SemanticIndex = 0;
	input_layout_desc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	input_layout_desc[1].InputSlot = 0;
	input_layout_desc[1].AlignedByteOffset = 12;
	input_layout_desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[1].InstanceDataStepRate = 0;

	HRESULT hr = Engine::Instance().device()->CreateInputLayout(
		input_layout_desc,
		sizeof(input_layout_desc) / sizeof(input_layout_desc[0]),
		vertex_shader_code.data(),
		vertex_shader_code.size(),
		&input_layout_);
	assert(SUCCEEDED(hr));
}

void Shader::AddShader(ShaderType shader_type, const wstring &filename)
{
	vector<char> shader_code = ReadFile(filename);
	HRESULT hr;
	switch (shader_type)
	{
	case ShaderType::VS:
		SafeRelease(vertex_shader_);
		hr = Engine::Instance().device()->CreateVertexShader(
			shader_code.data(), shader_code.size(), nullptr, &vertex_shader_);
		assert(SUCCEEDED(hr));
		InitInputLayout(shader_code);
		break;
	case ShaderType::HS:
		SafeRelease(hull_shader_);
		hr = Engine::Instance().device()->CreateHullShader(
			shader_code.data(), shader_code.size(), nullptr, &hull_shader_);
		assert(SUCCEEDED(hr));
		InitInputLayout(shader_code);
		break;
	case ShaderType::DS:
		SafeRelease(domain_shader_);
		hr = Engine::Instance().device()->CreateDomainShader(
			shader_code.data(), shader_code.size(), nullptr, &domain_shader_);
		assert(SUCCEEDED(hr));
		InitInputLayout(shader_code);
		break;
	case ShaderType::PS:
		SafeRelease(pixel_shader_);
		hr = Engine::Instance().device()->CreatePixelShader(
			shader_code.data(), shader_code.size(), nullptr, &pixel_shader_);
		assert(SUCCEEDED(hr));
		break;
	case ShaderType::CS:
		SafeRelease(compute_shader_);
		hr = Engine::Instance().device()->CreateComputeShader(
			shader_code.data(), shader_code.size(), nullptr, &compute_shader_);
		assert(SUCCEEDED(hr));
		break;
	}
}

void Shader::Use() const
{
	Engine::Instance().device_context()->VSSetShader(vertex_shader_, nullptr, 0);
	Engine::Instance().device_context()->HSSetShader(hull_shader_, nullptr, 0);
	Engine::Instance().device_context()->DSSetShader(domain_shader_, nullptr, 0);
	Engine::Instance().device_context()->PSSetShader(pixel_shader_, nullptr, 0);
	Engine::Instance().device_context()->CSSetShader(compute_shader_, nullptr, 0);
	Engine::Instance().device_context()->IASetInputLayout(input_layout_);
}

vector<char> Shader::ReadFile(const wstring& filename)
{
	vector<char> content;
	ifstream fin(filename, ios_base::binary);
	if (!fin)
		throw AppError(L"Cannot open shader file: " + filename);
	fin.seekg(0, ios_base::end);
	int file_size = fin.tellg();
	fin.seekg(0, ios_base::beg);
	content.resize(file_size);
	fin.read(content.data(), file_size);
	return content;
}