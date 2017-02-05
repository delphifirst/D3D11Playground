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
	void AddTexture(ShaderType shader, const std::wstring &filename);
	void UpdateCBuffer(int slot, void *data, int bytes);
	void Use();

private:
	ID3D11Buffer* vertex_buffer_;
	int vertex_stride_;
	std::vector<ID3D11Buffer*> vs_cbuffers_;
	std::vector<ID3D11ShaderResourceView*> ps_texture_views_;
};