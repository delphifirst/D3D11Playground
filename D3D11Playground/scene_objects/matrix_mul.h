#pragma once

#include <string>

#include "../engine/scene_object.h"

class MatrixMul : public SceneObject
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
	static constexpr int kMatrixSize = 32;
	float input_data_[kMatrixSize * kMatrixSize];
	float cpu_result_[kMatrixSize * kMatrixSize];
	float gpu_result_[kMatrixSize * kMatrixSize];
};