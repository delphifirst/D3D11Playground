#include "windows_input_layout.h"

#include "utils.h"

using namespace std;

namespace playground
{
	WindowsInputLayout::WindowsInputLayout(ID3D11InputLayout* input_layout)
		: input_layout_(input_layout)
	{

	}
	WindowsInputLayout::~WindowsInputLayout()
	{
		SafeRelease(input_layout_);
	}
}