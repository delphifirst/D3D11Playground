#pragma once

#include <string>
#include <vector>

#include <d3d11.h>

#include "utils.h"

template<typename T>
class Shader
{
public:
	Shader(const std::wstring& source_filename)
		: source_filename_(source_filename)
	{
		shader_code_ = ReadFile(source_filename);
	}
	virtual ~Shader()
	{
		SafeRelease(shader_);
	}
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	virtual void Use() const = 0;
protected:
	T* shader_ = nullptr;
	std::wstring source_filename_;
	std::vector<char> shader_code_;
};

void ClearAllShaders();

class VertexShader : public Shader<ID3D11VertexShader>
{
public:
	VertexShader(const std::wstring& source_filename);
	virtual ~VertexShader();

	virtual void Use() const;
	void UpdateInputLayout(D3D11_INPUT_ELEMENT_DESC input_elem_desc[], int input_elem_count);

private:
	ID3D11InputLayout* input_layout_ = nullptr;
};

class HullShader : public Shader<ID3D11HullShader>
{
public:
	HullShader(const std::wstring& source_filename);
	virtual void Use() const;
};

class DomainShader :public Shader<ID3D11DomainShader>
{
public:
	DomainShader(const std::wstring& source_filename);
	virtual void Use() const;
};

class GeometryShader :public Shader<ID3D11GeometryShader>
{
public:
	GeometryShader(const std::wstring& source_filename);
	virtual void Use() const;
};

class GeometrySOShader :public Shader<ID3D11GeometryShader>
{
public:
	GeometrySOShader(const std::wstring& source_filename);
	void UpdateOutputLayout(D3D11_SO_DECLARATION_ENTRY output_layout[], int output_elem_count, const std::vector<UINT>& strides);
	virtual void Use() const;
};

class PixelShader :public Shader<ID3D11PixelShader>
{
public:
	PixelShader(const std::wstring& source_filename);
	virtual void Use() const;
};

class ComputeShader :public Shader<ID3D11ComputeShader>
{
public:
	ComputeShader(const std::wstring& source_filename);
	virtual void Use() const;
};