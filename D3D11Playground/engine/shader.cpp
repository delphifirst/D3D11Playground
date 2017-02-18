#include "shader.h"

#include <cassert>
#include <fstream>

#include "utils.h"
#include "engine.h"

using namespace std;

void ClearAllShaders()
{
	Engine::Instance().device_context()->VSSetShader(nullptr, nullptr, 0);
	Engine::Instance().device_context()->HSSetShader(nullptr, nullptr, 0);
	Engine::Instance().device_context()->DSSetShader(nullptr, nullptr, 0);
	Engine::Instance().device_context()->GSSetShader(nullptr, nullptr, 0);
	Engine::Instance().device_context()->PSSetShader(nullptr, nullptr, 0);
	Engine::Instance().device_context()->CSSetShader(nullptr, nullptr, 0);
}

VertexShader::VertexShader(const wstring& source_filename)
	: Shader(source_filename)
{
	HRESULT hr = Engine::Instance().device()->CreateVertexShader(
		shader_code_.data(), shader_code_.size(), nullptr, &shader_);
	assert(SUCCEEDED(hr));
}

VertexShader::~VertexShader()
{
	SafeRelease(input_layout_);
}

void VertexShader::Use() const
{
	Engine::Instance().device_context()->IASetInputLayout(input_layout_);
	Engine::Instance().device_context()->VSSetShader(shader_, nullptr, 0);
}

void VertexShader::UpdateInputLayout(D3D11_INPUT_ELEMENT_DESC input_elem_desc[], int input_elem_count)
{
	SafeRelease(input_layout_);
	HRESULT hr = Engine::Instance().device()->CreateInputLayout(
		input_elem_desc, input_elem_count, shader_code_.data(), shader_code_.size(), &input_layout_);
	assert(SUCCEEDED(hr));
}

HullShader::HullShader(const wstring& source_filename)
	:Shader(source_filename)
{
	HRESULT hr = Engine::Instance().device()->CreateHullShader(
		shader_code_.data(), shader_code_.size(), nullptr, &shader_);
	assert(SUCCEEDED(hr));
}

void HullShader::Use() const
{
	Engine::Instance().device_context()->HSSetShader(shader_, nullptr, 0);
}

DomainShader::DomainShader(const wstring& source_filename)
	:Shader(source_filename)
{
	HRESULT hr = Engine::Instance().device()->CreateDomainShader(
		shader_code_.data(), shader_code_.size(), nullptr, &shader_);
	assert(SUCCEEDED(hr));
}

void DomainShader::Use() const
{
	Engine::Instance().device_context()->DSSetShader(shader_, nullptr, 0);
}

GeometryShader::GeometryShader(const wstring& source_filename)
	:Shader(source_filename)
{
	HRESULT hr = Engine::Instance().device()->CreateGeometryShader(
		shader_code_.data(), shader_code_.size(), nullptr, &shader_);
	assert(SUCCEEDED(hr));
}

void GeometryShader::Use() const
{
	Engine::Instance().device_context()->GSSetShader(shader_, nullptr, 0);
}

PixelShader::PixelShader(const wstring& source_filename)
	:Shader(source_filename)
{
	HRESULT hr = Engine::Instance().device()->CreatePixelShader(
		shader_code_.data(), shader_code_.size(), nullptr, &shader_);
	assert(SUCCEEDED(hr));
}

void PixelShader::Use() const
{
	Engine::Instance().device_context()->PSSetShader(shader_, nullptr, 0);
}

ComputeShader::ComputeShader(const wstring& source_filename)
	:Shader(source_filename)
{
	HRESULT hr = Engine::Instance().device()->CreateComputeShader(
		shader_code_.data(), shader_code_.size(), nullptr, &shader_);
	assert(SUCCEEDED(hr));
}

void ComputeShader::Use() const
{
	Engine::Instance().device_context()->CSSetShader(shader_, nullptr, 0);
}
