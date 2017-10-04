#pragma once

#include <vector>

#include <d3d11.h>

#include "input_layout.h"

namespace playground
{
	class WindowsInputLayout : public IInputLayout
	{
	public:
		WindowsInputLayout(ID3D11InputLayout* input_layout);
		virtual ~WindowsInputLayout() override;
		ID3D11InputLayout* GetInputLayout()
		{
			return input_layout_;
		}

	private:
		ID3D11InputLayout* input_layout_;
	};
}