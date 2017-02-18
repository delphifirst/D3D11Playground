#pragma once

#include <string>

#include "../engine/object.h"
#include "../engine/shader.h"
#include "../engine/render_state.h"
#include "../engine/resource.h"

class MatrixMul : public Object
{
public:
	struct CSBuffer
	{
		int size;
		int unused[3];
	};

	MatrixMul(const std::wstring& name);

	virtual void OnDraw() override;

private:
	Shader shader_;
	RenderState render_state_;
	Resource resource_;

	static constexpr int kMatrixSize = 32;
	float input_data_[kMatrixSize * kMatrixSize];
	float cpu_result_[kMatrixSize * kMatrixSize];
	float gpu_result_[kMatrixSize * kMatrixSize];
};