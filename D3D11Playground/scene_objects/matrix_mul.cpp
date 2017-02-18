#include "matrix_mul.h"

#include <random>
#include <chrono>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

constexpr int MatrixMul::kMatrixSize;

MatrixMul::MatrixMul(const wstring& name) : Object(name),
	compute_shader_(L"resources/matrix_mul_cs.cso")
{
	default_random_engine random_engine;
	uniform_real_distribution<> dist;
	for (int i = 0; i < kMatrixSize * kMatrixSize; ++i)
		input_data_[i] = dist(random_engine);
	resource_.AddCBuffer(ShaderType::CS, sizeof(CSBuffer));
	CSBuffer buffer = { kMatrixSize, 0, 0, 0 };
	resource_.UpdateCBuffer(ShaderType::CS, 0, &buffer, sizeof(buffer));
	resource_.AddStructuredBuffer(ShaderType::CS, input_data_, 4, kMatrixSize * kMatrixSize);
	resource_.AddRWStructuredBuffer(ShaderType::CS, nullptr, 4, kMatrixSize * kMatrixSize);
}

void MatrixMul::OnDraw()
{
	resource_.Use();
	render_state_.Use();
	ClearAllShaders();
	compute_shader_.Use();

	ZeroMemory(cpu_result_, sizeof(cpu_result_));
	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
	for (int k = 0; k < kMatrixSize; ++k)
	{
		for (int i = 0; i < kMatrixSize; ++i)
		{
			float r = input_data_[i * kMatrixSize + k];
			for (int j = 0; j < kMatrixSize; ++j)
				cpu_result_[i * kMatrixSize + j] += r * input_data_[k * kMatrixSize + j];
		}
	}
	cout << "CPU time: " << static_cast<chrono::duration<double>>(
		chrono::steady_clock::now() - start).count() << endl;

	start = chrono::steady_clock::now();
	Engine::Instance().device_context()->Dispatch(kMatrixSize * kMatrixSize / 256, 1, 1);

	resource_.DumpRWStructuredBuffer(ShaderType::CS, 0, 4, kMatrixSize * kMatrixSize, gpu_result_);

	cout << "GPU time: " << static_cast<chrono::duration<double>>(
		chrono::steady_clock::now() - start).count() << endl;

	for (int i = 0; i < kMatrixSize * kMatrixSize; ++i)
		if (abs(cpu_result_[i] - gpu_result_[i]) > 0.001)
		{
			cout << "ERROR: " << cpu_result_[i] << ", " << gpu_result_[i] << endl;
		}
}