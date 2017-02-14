#pragma once

#include <vector>
#include <string>

#include <d3d11.h>

#include "engine.h"
#include "utils.h"

class Resource
{
public:
	Resource() = default;
	~Resource();
	Resource(const Resource& other) = delete;
	Resource& operator=(const Resource& other) = delete;

	void AddCBuffer(ShaderType shader, int bytes);
	void UpdateCBuffer(ShaderType shader, int slot, void *data, int bytes);

	void AddVertexBuffer(void* vertex_data, UINT stride, int bytes, bool is_dynamic);
	template<typename T>
	T* MapVertexBuffer(int slot)
	{
		D3D11_MAPPED_SUBRESOURCE buffer_map = {};
		HRESULT hr = Engine::Instance().device_context()->Map(
			vertex_buffers_[slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &buffer_map);
		assert(SUCCEEDED(hr));
		return static_cast<T*>(buffer_map.pData);
	}
	void UnmapVertexBuffer(int slot);
	void UpdateVertexBuffer(int slot, void* vertex_data, int bytes);

	void AddTexture(ShaderType shader, const std::wstring &filename);
	void AddTexture(ShaderType shader, int width, int height, DXGI_FORMAT format, void* data);
	void AddStructuredBuffer(ShaderType shader, void* data, int elem_bytes, int elem_count);
	void AddRWStructuredBuffer(ShaderType shader, void* data, int elem_bytes, int elem_count);
	ID3D11UnorderedAccessView* GetRWStructuredBuffer(ShaderType shader, int slot);

	void Use();

private:
	ID3D11Buffer* CreateStructuredBuffer(void* data, int elem_bytes, int elem_count);

	std::vector<ID3D11Buffer*> vertex_buffers_;
	std::vector<UINT> vertex_buffer_strides_;
	std::vector<UINT> vertex_buffer_offsets_;

	std::vector<ID3D11Buffer*> vs_cbuffers_;
	std::vector<ID3D11Buffer*> ds_cbuffers_;
	std::vector<ID3D11Buffer*> cs_cbuffers_;
	std::vector<ID3D11Buffer*> gs_cbuffers_;
	std::vector<ID3D11ShaderResourceView*> ps_shader_resource_views_;
	std::vector<ID3D11ShaderResourceView*> cs_shader_resource_views_;
	std::vector<ID3D11UnorderedAccessView*> cs_unordered_access_views_;
};