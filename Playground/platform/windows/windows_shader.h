#pragma once

#include <string>
#include <vector>

#include <d3d11.h>

#include "shader.h"

namespace playground
{
	template<typename T>
	class WindowsShader :public IShader
	{
	public:
		WindowsShader(const std::string& name, const std::vector<char>& code, T* shader);
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

	private:
		std::string name_;
		std::vector<char> code_;
		T* shader_;
	};

	template<typename T>
	WindowsShader<T>::WindowsShader(const std::string& name, const std::vector<char>& code, T* shader)
		: name_(name), code_(code), shader_(shader)
	{

	}
	template<typename T>
	WindowsShader<T>::~WindowsShader()
	{
		SafeRelease(shader_);
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