#pragma once

#include <vector>
#include <string>

#include <d3d11.h>

#include "utils.h"

class Resource
{
public:
	Resource(void *vertex_data, int vertex_stride, int total_bytes);
	~Resource();
	Resource(const Resource& other) = delete;
	Resource& operator=(const Resource& other) = delete;

	void AddCBuffer(ShaderType shader, int bytes);
	void UpdateCBuffer(ShaderType shader, int slot, void *data, int bytes);

	void AddTexture(ShaderType shader, const std::wstring &filename);
	void AddTexture(ShaderType shader, int width, int height, DXGI_FORMAT format, void* data);
	void AddStructuredBuffer(ShaderType shader, void* data, int elem_bytes, int elem_count);
	void AddRWStructuredBuffer(ShaderType shader, void* data, int elem_bytes, int elem_count);
	ID3D11UnorderedAccessView* GetRWStructuredBuffer(ShaderType shader, int slot);

	void Use();

private:
	ID3D11Buffer* CreateStructuredBuffer(void* data, int elem_bytes, int elem_count);

	ID3D11Buffer* vertex_buffer_;
	int vertex_stride_;
	std::vector<ID3D11Buffer*> vs_cbuffers_;
	std::vector<ID3D11Buffer*> ds_cbuffers_;
	std::vector<ID3D11Buffer*> cs_cbuffers_;
	std::vector<ID3D11ShaderResourceView*> ps_shader_resource_views_;
	std::vector<ID3D11ShaderResourceView*> cs_shader_resource_views_;
	std::vector<ID3D11UnorderedAccessView*> cs_unordered_access_views_;
};