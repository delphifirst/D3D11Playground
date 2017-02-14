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
	SafeRelease(geometry_shader_);
	SafeRelease(pixel_shader_);
	SafeRelease(compute_shader_);
	SafeRelease(input_layout_);
}

void Shader::AddShader(ShaderType shader_type, const wstring &filename)
{
	vector<char> shader_code = ReadFile(filename);
	HRESULT hr;
	switch (shader_type)
	{
	case ShaderType::HS:
		SafeRelease(hull_shader_);
		hr = Engine::Instance().device()->CreateHullShader(
			shader_code.data(), shader_code.size(), nullptr, &hull_shader_);
		assert(SUCCEEDED(hr));
		break;
	case ShaderType::DS:
		SafeRelease(domain_shader_);
		hr = Engine::Instance().device()->CreateDomainShader(
			shader_code.data(), shader_code.size(), nullptr, &domain_shader_);
		assert(SUCCEEDED(hr));
		break;
	case ShaderType::GS:
		SafeRelease(geometry_shader_);
		hr = Engine::Instance().device()->CreateGeometryShader(
			shader_code.data(), shader_code.size(), nullptr, &geometry_shader_);
		assert(SUCCEEDED(hr));
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

void Shader::AddVertexShader(const wstring &filename, D3D11_INPUT_ELEMENT_DESC input_elem_desc[], int input_elem_count)
{
	vector<char> shader_code = ReadFile(filename);
	HRESULT hr;

	SafeRelease(vertex_shader_);
	hr = Engine::Instance().device()->CreateVertexShader(
		shader_code.data(), shader_code.size(), nullptr, &vertex_shader_);
	assert(SUCCEEDED(hr));

	hr = Engine::Instance().device()->CreateInputLayout(
		input_elem_desc, input_elem_count, shader_code.data(), shader_code.size(), &input_layout_);
	assert(SUCCEEDED(hr));
}

void Shader::Use() const
{
	Engine::Instance().device_context()->VSSetShader(vertex_shader_, nullptr, 0);
	Engine::Instance().device_context()->HSSetShader(hull_shader_, nullptr, 0);
	Engine::Instance().device_context()->DSSetShader(domain_shader_, nullptr, 0);
	Engine::Instance().device_context()->GSSetShader(geometry_shader_, nullptr, 0);
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