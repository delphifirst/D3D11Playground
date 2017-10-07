#pragma once

#include <string>
#include <vector>
#include <map>
#include <cassert>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "shader.h"

namespace playground
{
	template<typename T>
	class WindowsShader :public IShader
	{
	public:
		WindowsShader(const std::string& name, const std::vector<char>& code, T* shader, 
			ID3D11ShaderReflection* shader_reflection, ID3D11Buffer* constant_buffer, int constant_buffer_size);
		virtual ~WindowsShader() override;

		virtual std::string GetName() const override
		{
			return name_;
		}
		virtual ShaderType GetType() const override
		{
			return UNKNOWN_SHADER;
		}

		std::vector<char> GetCode() const
		{
			return code_;
		}
		T* GetShader()
		{
			return shader_;
		}
		int GetConstantBufferSlot() const
		{
			return constant_buffer_slot_;
		}
		const std::vector<char>& GetCpuConstantBuffer() const
		{
			return cpu_constant_buffer_;
		}
		ID3D11Buffer* GetGpuConstantBuffer()
		{
			return gpu_constant_buffer_;
		}

		virtual void SetVariable(const std::string& name, const void* data) override;

	private:
		std::string name_;
		std::vector<char> code_;
		T* shader_;
		std::map<std::string, std::pair<int, int>> variable_offset_size_;
		ID3D11ShaderReflection* shader_reflection_;
		std::vector<char> cpu_constant_buffer_;
		ID3D11Buffer* gpu_constant_buffer_;
		int constant_buffer_slot_;
	};

	template<typename T>
	WindowsShader<T>::WindowsShader(const std::string& name, const std::vector<char>& code, T* shader,
		ID3D11ShaderReflection* shader_reflection, ID3D11Buffer* constant_buffer, int constant_buffer_size)
		: name_(name), code_(code), shader_(shader), shader_reflection_(shader_reflection), 
		gpu_constant_buffer_(constant_buffer)
	{
		cpu_constant_buffer_.resize(constant_buffer_size);
		if (shader_reflection)
		{
			D3D11_SHADER_INPUT_BIND_DESC desc;
			shader_reflection->GetResourceBindingDescByName("Global", &desc);
			constant_buffer_slot_ = desc.BindPoint;
		}
		else
		{
			constant_buffer_slot_ = -1;
		}
	}
	template<typename T>
	WindowsShader<T>::~WindowsShader()
	{
		SafeRelease(shader_reflection_);
		SafeRelease(gpu_constant_buffer_);
		SafeRelease(shader_);
	}

	template<typename T>
	void WindowsShader<T>::SetVariable(const std::string& name, const void* data)
	{
		int offset, size;
		auto it = variable_offset_size_.find(name);
		if (it == variable_offset_size_.end())
		{
			ID3D11ShaderReflectionConstantBuffer* constant_reflection = shader_reflection_->GetConstantBufferByName("Global");
			ID3D11ShaderReflectionVariable* variable_reflection = constant_reflection->GetVariableByName(name.c_str());
			D3D11_SHADER_VARIABLE_DESC desc;
			HRESULT hr = variable_reflection->GetDesc(&desc);
			assert(SUCCEEDED(hr));
			offset = desc.StartOffset;
			size = desc.Size;
			variable_offset_size_[name] = std::make_pair(offset, size);
		}
		else
		{
			offset = it->second.first;
			size = it->second.second;
		}
		memcpy(cpu_constant_buffer_.data() + offset, data, size);
	}

	template<>
	ShaderType WindowsShader<ID3D11VertexShader>::GetType() const
	{
		return VERTEX_SHADER;
	}
	template<>
	ShaderType WindowsShader<ID3D11PixelShader>::GetType() const
	{
		return PIXEL_SHADER;
	}
}