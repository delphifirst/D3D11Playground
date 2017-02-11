#include "quad.h"

#include <cassert>
#include <random>
#include <chrono>
#include <iostream>
#include <cmath>

#include "../engine/engine.h"

using namespace std;
using namespace DirectX;

constexpr int Quad::kMatrixSize;

Quad::Quad(const wstring& name, void *vertex_data, int bytes,
	const wstring &texture_filename)
	: SceneObject(name, vertex_data, sizeof(Vertex), bytes)
{
	resource_.AddCBuffer(ShaderType::DS, sizeof(MatrixBuffer));
	resource_.AddTexture(ShaderType::PS, texture_filename);

	D3D11_INPUT_ELEMENT_DESC input_layout_desc[2];

	input_layout_desc[0].SemanticName = "POSITION";
	input_layout_desc[0].SemanticIndex = 0;
	input_layout_desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	input_layout_desc[0].InputSlot = 0;
	input_layout_desc[0].AlignedByteOffset = 0;
	input_layout_desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[0].InstanceDataStepRate = 0;

	input_layout_desc[1].SemanticName = "TEXCOORD";
	input_layout_desc[1].SemanticIndex = 0;
	input_layout_desc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	input_layout_desc[1].InputSlot = 0;
	input_layout_desc[1].AlignedByteOffset = 12;
	input_layout_desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	input_layout_desc[1].InstanceDataStepRate = 0;

	shader_.AddVertexShader(L"resources/quad_vs.cso", input_layout_desc, 
		sizeof(input_layout_desc) / sizeof(input_layout_desc[0]));
	shader_.AddShader(ShaderType::HS, L"resources/quad_hs.cso");
	shader_.AddShader(ShaderType::DS, L"resources/quad_ds.cso");
	shader_.AddShader(ShaderType::PS, L"resources/quad_ps.cso");

	InitComputeShaderTest();
}

void Quad::InitComputeShaderTest()
{
	shader_.AddShader(ShaderType::CS, L"resources/quad_cs.cso");
	default_random_engine random_engine;
	uniform_real_distribution<> dist;
	for (int i = 0; i < kMatrixSize * kMatrixSize; ++i)
		input_data[i] = dist(random_engine);
	resource_.AddCBuffer(ShaderType::CS, sizeof(CSBuffer));
	CSBuffer buffer = { kMatrixSize, 0, 0, 0 };
	resource_.UpdateCBuffer(ShaderType::CS, 0, &buffer, sizeof(buffer));
	resource_.AddStructuredBuffer(ShaderType::CS, input_data, 4, kMatrixSize * kMatrixSize);
	resource_.AddRWStructuredBuffer(ShaderType::CS, nullptr, 4, kMatrixSize * kMatrixSize);
}

void Quad::ComputeShaderTest()
{
	ZeroMemory(cpu_result, sizeof(cpu_result));
	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
	for (int k = 0; k < kMatrixSize; ++k)
	{
		for (int i = 0; i < kMatrixSize; ++i)
		{
			float r = input_data[i * kMatrixSize + k];
			for (int j = 0; j < kMatrixSize; ++j)
				cpu_result[i * kMatrixSize + j] += r * input_data[k * kMatrixSize + j];
		}
	}
	cout << "CPU time: " << static_cast<chrono::duration<double>>(
		chrono::steady_clock::now() - start).count() << endl;

	start = chrono::steady_clock::now();
	Engine::Instance().device_context()->Dispatch(kMatrixSize * kMatrixSize / 256, 1, 1);

	ID3D11Buffer* gpu_result_buffer = nullptr;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = 4 * kMatrixSize * kMatrixSize;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = 4;
	HRESULT hr = Engine::Instance().device()->CreateBuffer(&desc, nullptr, &gpu_result_buffer);
	assert(SUCCEEDED(hr));
	ID3D11Resource* gpu_result_buffer_on_gpu = nullptr;
	resource_.GetRWStructuredBuffer(ShaderType::CS, 0)->GetResource(&gpu_result_buffer_on_gpu);
	Engine::Instance().device_context()->CopyResource(gpu_result_buffer, gpu_result_buffer_on_gpu);
	SafeRelease(gpu_result_buffer_on_gpu);
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	hr = Engine::Instance().device_context()->Map(gpu_result_buffer, 0, D3D11_MAP_READ, 0, &mapped_resource);
	assert(SUCCEEDED(hr));
	memcpy(gpu_result, mapped_resource.pData, sizeof(gpu_result));
	Engine::Instance().device_context()->Unmap(gpu_result_buffer, 0);
	SafeRelease(gpu_result_buffer);

	cout << "GPU time: " << static_cast<chrono::duration<double>>(
		chrono::steady_clock::now() - start).count() << endl;

	for (int i = 0; i < kMatrixSize * kMatrixSize; ++i)
		if (abs(cpu_result[i] - gpu_result[i]) > 0.001)
		{
			cout << "ERROR: " << cpu_result[i] << ", " << gpu_result[i] << endl;
		}
}

void Quad::OnUpdate(double delta_time)
{
	SceneObject::OnUpdate(delta_time);
}

void Quad::OnDraw()
{
	SceneObject::OnDraw();

	ComputeShaderTest();

	MatrixBuffer matrix_buffer;
	XMMATRIX transposed_matrix;

	transposed_matrix = XMLoadFloat4x4(&world_transform());
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.world_matrix, transposed_matrix);

	transposed_matrix = Engine::Instance().main_camera()->GetViewMatrix();
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.view_matrix, transposed_matrix);

	transposed_matrix = Engine::Instance().main_camera()->GetProjectMatrix(
		Engine::Instance().width(), Engine::Instance().height());
	transposed_matrix = XMMatrixTranspose(transposed_matrix);
	XMStoreFloat4x4(&matrix_buffer.proj_matrix, transposed_matrix);

	resource_.UpdateCBuffer(ShaderType::DS, 0, &matrix_buffer, sizeof(matrix_buffer));

	Engine::Instance().device_context()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	Engine::Instance().device_context()->Draw(4, 0);
}

Quad* XM_CALLCONV MakeQuad(const wstring& name, FXMVECTOR a, FXMVECTOR b,
	FXMVECTOR c, GXMVECTOR d, const wstring& texture_filename)
{
	Quad::Vertex vertices[4];
	XMStoreFloat3(&vertices[0].position, a);
	vertices[0].tex_coord = XMFLOAT2(0, 0);
	XMStoreFloat3(&vertices[1].position, b);
	vertices[1].tex_coord = XMFLOAT2(1, 0);
	XMStoreFloat3(&vertices[2].position, c);
	vertices[2].tex_coord = XMFLOAT2(0, 1);
	XMStoreFloat3(&vertices[3].position, d);
	vertices[3].tex_coord = XMFLOAT2(1, 1);
	return new Quad(name, vertices, sizeof(vertices), texture_filename);
}